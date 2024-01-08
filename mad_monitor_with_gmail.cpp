//needs POCO library installed on system
#include <iostream>
#include <string>
#include <filesystem>
#include "Poco/Net/MailMessage.h"
#include "Poco/Net/SMTPClientSession.h"
#include "Poco/Net/StringPartSource.h"
#include "Poco/Net/MailRecipient.h"
#include "Poco/Net/SecureSMTPClientSession.h"

using namespace std;
using namespace std::filesystem;
using namespace Poco::Net;

//configuring SMTP server
const string smtpServer = "smtp.gmail.com";
const int smtpPort = 465; //-->SSL/TLS port for Gmail
const string smtpUsername = "your-gmail-account@gmail.com";
const string smtpPassword = "your-gmail-password";

//email message configuration
const string from = "your-gmail-account@gmail.com";
const string to = "recipient-email-address@gmail.com";
const string subject = "Directory Modification Alert";

void sendEmail(const string& body)
{
    try
    {
        //creating a new mail message
        MailMessage message;
        message.setSender(from);
        message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
        message.setSubject(subject);
        message.setContentType("text/plain");

        //attaching the email body as a string part source
        message.addPart(new StringPartSource(body));

        //creating a secure SMTP client session with the Gmail SMTP server
        SecureSMTPClientSession session(smtpServer, smtpPort);
        session.login(SMTPClientSession::AUTH_LOGIN, smtpUsername, smtpPassword);

        //sending the email message
        session.sendMessage(message);

        //logging out from the SMTP server
        session.close();
    }
    catch (Exception& ex)
    {
        cerr << "error sending email--> " << ex.displayText() << endl;
    }
}

int main()
{
    //entering the directory path to monitor for modifications
    string dirPath = "C:\\Users\\Username\\Documents\\MyFolder";

    //initial file count for the directory
    int fileCount = distance(directory_iterator(dirPath), directory_iterator{});

    while (true)
    {
        //getting the current file count for the directory
        int currentCount = distance(directory_iterator(dirPath), directory_iterator{});

        //if the file count has changed,sending an email alert
        if (currentCount != fileCount)
        {
            //sendEmail
            string body = "The directory '" + dirPath + "' has been modified.";
            sendEmail(body);

            //updating the file count for the directory
            fileCount = currentCount;
        }

        //waiting for 10 seconds before checking again
        this_thread::sleep_for(chrono::seconds(10));
    }

    return 0;
}
