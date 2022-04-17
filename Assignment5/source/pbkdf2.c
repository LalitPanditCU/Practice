/*
 * pbkdf2.c
 *
 * A perfectly legitimate implementation of HMAC and PBKDF2, but based
 * on the "ISHA" insecure and bad hashing algorithm.
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 */

#include <assert.h>

#include "pbkdf2.h"

int cmp_str(const uint8_t *b1, size_t lastlen, const uint8_t *b2, size_t len)
{

  if (lastlen != len) return 0;

  for (size_t i=0; i<len; i++)
    if (b1[i] != b2[i])
      return 0;
  return 1;
}

void cpy_str(uint8_t *d, const uint8_t *s, size_t len)
{

	  for (size_t i=0; i<len; i++)
	    d[i] = s[i];
}

/*
 * See function description in pbkdf2.h
 */
void hmac_isha(const uint8_t *key, size_t key_len,
    const uint8_t *msg, size_t msg_len,
    uint8_t *digest)
{
  uint8_t ipad[ISHA_BLOCKLEN];
  uint8_t opad[ISHA_BLOCKLEN];
  uint8_t keypad[ISHA_BLOCKLEN];
  uint8_t inner_digest[ISHA_DIGESTLEN];
  size_t i;
  ISHAContext ctx;

  static ISHAContext ipad_ctx, opad_ctx;
  static uint8_t lastkey[ISHA_BLOCKLEN];
  static size_t lastkey_len;

  if (cmp_str(lastkey, lastkey_len, key, key_len) == 0)
  {
	  cpy_str(lastkey, key, ISHA_BLOCKLEN);
	  lastkey_len = key_len;

	  if (key_len > ISHA_BLOCKLEN) {
		// If key_len > ISHA_BLOCKLEN reset it to key=ISHA(key)
		ISHAReset(&ctx);
		ISHAInput(&ctx, key, key_len);
		ISHAResult(&ctx, keypad);

	  } else {
		// key_len <= ISHA_BLOCKLEN; copy key into keypad, zero pad the result
		for (i=0; i<key_len; i++)
		  keypad[i] = key[i];
		for(i=key_len; i<ISHA_BLOCKLEN; i++)
		  keypad[i] = 0x00;
	  }

	  // XOR key into ipad and opad
	  for (i=0; i<ISHA_BLOCKLEN; i++) {
		ipad[i] = keypad[i] ^ 0x36;
		opad[i] = keypad[i] ^ 0x5c;
	  }

	  // Perform inner ISHA
	  ISHAReset(&ctx);
	  ISHAInput(&ctx, ipad, ISHA_BLOCKLEN);
	  cpy_str((uint8_t *)&ipad_ctx, (uint8_t *)&ctx, sizeof(ctx));

	  ISHAInput(&ctx, msg, msg_len);
	  ISHAResult(&ctx, inner_digest);

	  // perform outer ISHA
	  ISHAReset(&ctx);
	  ISHAInput(&ctx, opad, ISHA_BLOCKLEN);
	  cpy_str((uint8_t *)&opad_ctx, (uint8_t *)&ctx, sizeof(ctx));

	  ISHAInput(&ctx, inner_digest, ISHA_DIGESTLEN);
	  ISHAResult(&ctx, digest);
  }
  else
  {
	  cpy_str((uint8_t *)&ctx, (uint8_t *)&ipad_ctx, sizeof(ctx));
	  ISHAInput(&ctx, msg, msg_len);
	  ISHAResult(&ctx, inner_digest);


	  cpy_str((uint8_t *)&ctx, (uint8_t *)&opad_ctx, sizeof(ctx));
	  ISHAInput(&ctx, inner_digest, ISHA_DIGESTLEN);
	  ISHAResult(&ctx, digest);
  }
}


/*
 * Implements the F function as defined in RFC 8018 section 5.2
 *
 * Parameters:
 *   pass      The password
 *   pass_len  length of pass
 *   salt      The salt
 *   salt_len  length of salt
 *   iter      The iteration count ("c" in RFC 8018)
 *   blkidx    the block index ("i" in RFC 8018)
 *   result    The result, which is ISHA_DIGESTLEN bytes long
 * 
 * Returns:
 *   The result of computing the F function, in result
 */
static void F(const uint8_t *pass, size_t pass_len,
    const uint8_t *salt, size_t salt_len,
    int iter, unsigned int blkidx, uint8_t *result)
{
  uint8_t temp[ISHA_DIGESTLEN];
  uint8_t saltplus[2048];
  size_t i;
  assert(salt_len + 4 <= sizeof(saltplus));

  for (i=0; i<salt_len; i++)
    saltplus[i] = salt[i];

  // append blkidx in 4 bytes big endian 
  saltplus[i] = (blkidx & 0xff000000) >> 24;
  saltplus[i+1] = (blkidx & 0x00ff0000) >> 16;
  saltplus[i+2] = (blkidx & 0x0000ff00) >> 8;
  saltplus[i+3] = (blkidx & 0x000000ff);

  hmac_isha(pass, pass_len, saltplus, salt_len+4, temp);
  for (int i=0; i<ISHA_DIGESTLEN; i++)
    result[i] = temp[i];

  for (int j=1; j<iter; j++) {
    hmac_isha(pass, pass_len, temp, ISHA_DIGESTLEN, temp);
    for (int i=0; i<ISHA_DIGESTLEN; i++)
      result[i] ^= temp[i];
  }
}


/*
 * See function description in pbkdf2.h
 */
void pbkdf2_hmac_isha(const uint8_t *pass, size_t pass_len,
    const uint8_t *salt, size_t salt_len, int iter, size_t dkLen, uint8_t *DK)
{
  uint8_t accumulator[2560];
  assert(dkLen < sizeof(accumulator));

  int l = dkLen / ISHA_DIGESTLEN + 1;
  for (int i=0; i<l; i++) {
    F(pass, pass_len, salt, salt_len, iter, i+1, accumulator + i*ISHA_DIGESTLEN);
  }
  for (size_t i=0; i<dkLen; i++) {
    DK[i] = accumulator[i];
  }
}



