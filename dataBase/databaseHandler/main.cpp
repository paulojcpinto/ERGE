#include <QCoreApplication>
#include "sqlquery.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SqlQuery ola;
    fullUser user;
    user.user.Email="andrecampos1998@gmail.com";
    user.user.PinCode="1237";
    user.user.NickName="PAULO";
    user.user.PhoneNumber="913753546";
    user.user.EmailPassword="123456";
    user.faceInfo.PathDataset="/datasets";
    user.faceInfo.NumberOfImages=15;
    user.fingerInfo.FingerprintID=90;
    user.fingerInfo.FingerprintName="Dedo1";
    user.messageInfo.JumpTime=1000;
    user.messageInfo.UserMessage="Ola mundo, esta frio hoje!";
    user.messageInfo.dateToStart="30/3/2020";
    user.messageInfo.TargetPlatform="SMS 913753546";
    if(ola.insertUser(user))
        cout<<"UpsDeu\n";
    else cout<< "nao deu;\n";

    userQuery test;
    test.NickName="PAULO";
    if(ola.getUser("PAULO",&test))
        cout<< "Worked\n";
    else cout <<"didnt worked :(\n";

   cout <<"user parsing:"<<test.NickName<<","<<test.PinCode<<","<<test.Email<<", "<<test.PhoneNumber<<", "<<test.EmailPassword<<endl;

   faceQuery test1;
   ola.getFace("PAULO",&test1);
   cout<<"Face Parsing:"<<test1.PathDataset<<","<<test1.NumberOfImages<<endl;

   fingerPrintQuery test2;
   ola.getFingerprint("DrChip",&test2);
   cout<<"Finger Parsing:"<<test2.FingerprintID<<","<<test2.FingerprintName<<endl;

   messageInfoQuery test3;
   ola.getMessage("PAULO",&test3);
   cout<<"Message parsing:"<<test3.UserMessage<<","<<test3.TargetPlatform<<","<<test3.dateToStart<<","<<test3.JumpTime<<endl;

   fullUser test4;
   ola.getUser("PAULO",&test4);
   cout<<"FULL USER parsing:"<<test4.user.NickName<<","<<test4.user.PhoneNumber<<","<<test4.faceInfo.PathDataset<<","<<test4.fingerInfo.FingerprintID<<","<<test4.messageInfo.UserMessage<<endl;
    return 0;
}
