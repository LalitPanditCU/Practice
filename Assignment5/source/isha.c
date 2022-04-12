/*
 * isha.c
 *
 * A completely insecure and bad hashing algorithm, based loosely on
 * SHA-1 (which is itself no longer considered a good hashing
 * algorithm)
 *
 * Based on code for sha1 processing from Paul E. Jones, available at
 * https://www.packetizer.com/security/sha1/
 */

#include "isha.h"


/*
 * circular shift macro
 */
#define ISHACircularShift(bits,word) \
  (((word) << (bits)) | ((word) >> (32-(bits))))

#define MAX_LENGTH (2 * 0x1FFFFFFF)
#define MAX_LOW_LENGTH 0x1FFFFFFF

/*  
 * Processes the next 512 bits of the message stored in the MBlock
 * array.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAProcessMessageBlock(ISHAContext *ctx)
{
  uint32_t temp; 
  int t, t4;
  uint32_t W[16];
  uint8_t  *WB_ptr = (uint8_t *)W;
  uint32_t A, B, C, D, E;

  A = ctx->MD[0];
  B = ctx->MD[1];
  C = ctx->MD[2];
  D = ctx->MD[3];
  E = ctx->MD[4];

  for(t4 = 0, t=0; t4 < 64; t4 += 4, t++)
  {
    WB_ptr[t4+3] = ((uint8_t) ctx->MBlock[t4]);
    WB_ptr[t4+2] = ((uint8_t) ctx->MBlock[t4 + 1]);
    WB_ptr[t4+1] = ((uint8_t) ctx->MBlock[t4 + 2]);
    WB_ptr[t4] = ((uint8_t) ctx->MBlock[t4 + 3]);

    temp = ISHACircularShift(5,A) + ((B & C) | ((~B) & D)) + E + W[t];
    E = D;
    D = C;
    C = ISHACircularShift(30,B);
    B = A;
    A = temp;
  }

  ctx->MD[0] += A;
  ctx->MD[1] += B;
  ctx->MD[2] += C;
  ctx->MD[3] += D;
  ctx->MD[4] += E;

  ctx->MB_Idx = 0;
}


/*  
 * The message must be padded to an even 512 bits.  The first padding
 * bit must be a '1'.  The last 64 bits represent the length of the
 * original message.  All bits in between should be 0. This function
 * will pad the message according to those rules by filling the MBlock
 * array accordingly. It will also call ISHAProcessMessageBlock()
 * appropriately. When it returns, it can be assumed that the message
 * digest has been computed.
 *
 * Parameters:
 *   ctx         The ISHAContext (in/out)
 */
static void ISHAPadMessage(ISHAContext *ctx)
{
  /*
   *  Check to see if the current message block is too small to hold
   *  the initial padding bits and length.  If so, we will pad the
   *  block, process it, and then continue padding into a second
   *  block.
   */

  uint8_t *h_ptr = (uint8_t *)&(ctx->Length_High);
  uint8_t *l_ptr = (uint8_t *)&(ctx->Length_Low);


  if (ctx->MB_Idx > 55)
  {
    ctx->MBlock[ctx->MB_Idx++] = 0x80;
    while(ctx->MB_Idx < 64)
    {
      ctx->MBlock[ctx->MB_Idx++] = 0;
    }

    ISHAProcessMessageBlock(ctx);

    while(ctx->MB_Idx < 56)
    {
      ctx->MBlock[ctx->MB_Idx++] = 0;
    }
  }
  else
  {
    ctx->MBlock[ctx->MB_Idx++] = 0x80;
    while(ctx->MB_Idx < 56)
    {
      ctx->MBlock[ctx->MB_Idx++] = 0;
    }
  }

  /*
   *  Store the message length as the last 8 octets
   */
  ctx->MBlock[56] = h_ptr[3];
  ctx->MBlock[57] = h_ptr[2];
  ctx->MBlock[58] = h_ptr[1];
  ctx->MBlock[59] = h_ptr[0];
  ctx->MBlock[60] = l_ptr[3];
  ctx->MBlock[61] = l_ptr[2];
  ctx->MBlock[62] = l_ptr[1];
  ctx->MBlock[63] = l_ptr[0];

  ISHAProcessMessageBlock(ctx);
}


void ISHAReset(ISHAContext *ctx)
{
  ctx->Length_Low  = 0;
  ctx->Length_High = 0;
  ctx->MB_Idx      = 0;

  ctx->MD[0]       = 0x67452301;
  ctx->MD[1]       = 0xEFCDAB89;
  ctx->MD[2]       = 0x98BADCFE;
  ctx->MD[3]       = 0x10325476;
  ctx->MD[4]       = 0xC3D2E1F0;

  ctx->Computed    = 0;
  ctx->Corrupted   = 0;
}


void ISHAResult(ISHAContext *ctx, uint8_t *digest_out)
{

  uint8_t *md_ptr = (uint8_t *)ctx->MD;

  if (ctx->Corrupted)
  {
    return;
  }

  if (!ctx->Computed)
  {
    ISHAPadMessage(ctx);
    ctx->Computed = 1;
  }

  for (int i=0; i<20; i+=4) {

    digest_out[i]   = md_ptr[i+3];
    digest_out[i+1] = md_ptr[i+2];
    digest_out[i+2] = md_ptr[i+1];
    digest_out[i+3] = md_ptr[i];
  }

  return;
}


void ISHAInput(ISHAContext *ctx, const uint8_t *message_array, size_t length)
{
  if (!length)
  {
    return;
  }

  if (ctx->Computed || ctx->Corrupted)
  {
    ctx->Corrupted = 1;
    return;
  }

  uint32_t length_low_bytes = ctx->Length_Low >> 3;
  uint32_t length_high_bytes = ctx->Length_High >> 3;

  if (length + length_low_bytes + length_high_bytes  > MAX_LENGTH)
  {
	  ctx->Corrupted = 1;

	  return;
  }

  if (length + length_low_bytes > MAX_LOW_LENGTH)
  {
	  ctx->Length_High += (length << 3) + ctx->Length_Low - 0xFFFFFFFF;
	  ctx->Length_Low = 0xFFFFFFFF;
  }
  else
  {
	  ctx->Length_Low += (length << 3);
	  ctx->Length_High = 0;
  }

  while(length-- && !ctx->Corrupted)
  {
    ctx->MBlock[ctx->MB_Idx++] = *message_array;

    if (ctx->MB_Idx == 64)
    {
      ISHAProcessMessageBlock(ctx);
    }

    message_array++;
  }
}


