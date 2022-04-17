import os
import glob
import win32com.client as win32

dirname = '.'
names = 'names.txt'

outlook = win32.Dispatch('outlook.application')

send_account = None
for account in outlook.Session.Accounts:
    if 'colorado.edu' in account.DisplayName:
        send_account = account
        break

def send_mail(dir, fname):
    mail = outlook.CreateItem(0)
    mail._oleobj_.Invoke(*(64209, 0, 8, 0, send_account))

    mail.To = fname + '@colorado.edu'
    mail.Subject = 'CU PES login information'

    mail.Body = 'Login information for the vm:\n\n'
    mail.Body += '- Save the attached private key file. Do not share this file. The filename is your user name.\n'
    mail.Body += '- Login Command: ssh -i <saved_filepath>  ' +   fname + '@cupes.inovonics.com\n'
    mail.Body += '- Copy  Command: scp -i <saved_filepath>   <source>   <destination>.\n'
    mail.Body += '- For cupes files or directories, source or destination should be ' + fname + '@cupes.inovonics.com:<dir/filepath>\n'
    # mail.HTMLBody = '<h2>HTML Message body</h2>' #this field is optional

    # To attach a file to the email (optional):
    attachment  = dir + "\\" + fname
    attachment = os.path.abspath(attachment)
    mail.Attachments.Add(attachment)

    mail.Send()

def get_file_names():

    with open (names) as f:
        files = f.readlines()

    return [f.strip() for f in files]

for f in get_file_names():
    send_mail(dirname, f)