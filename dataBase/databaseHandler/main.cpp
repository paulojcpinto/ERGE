#include <QCoreApplication>
#include "sqlquery.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    SqlQuery ola;
    fullUser user;
    user.user.Email="andrecampos1998@gmail.com";
    user.user.PinCode="1237";
    user.user.NickName="ANDRE";
    user.user.PhoneNumber="913753546";
    user.user.EmailPassword="123456";
    user.faceInfo.PathDataset="/datasets";
    user.faceInfo.NumberOfImages=15;
    user.fingerInfo.FingerprintID=40;
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
   ola.getFingerprint("PAULO",&test2);
   cout<<"Finger Parsing:"<<test2.FingerprintID<<","<<test2.FingerprintName<<endl;

   messageInfoQuery test3;
   ola.getMessage("PAULO",&test3);
   cout<<"Message parsing:"<<test3.UserMessage<<","<<test3.TargetPlatform<<","<<test3.dateToStart<<","<<test3.JumpTime<<endl;

   fullUser test4;
   ola.getUser("PAULO",&test4);
   cout<<"FULL USER parsing:"<<test4.user.NickName<<","<<test4.user.PhoneNumber<<","<<test4.faceInfo.PathDataset<<","<<test4.fingerInfo.FingerprintID<<","<<test4.messageInfo.UserMessage<<endl;
   vector<fullUser> test5;
   test5=ola.getAllUsers();
   for(unsigned int i=0 ; i< test5.size();i++)
   {
       cout<<"USER "<<i<<": "<<test5.at(i).user.NickName<<"Message: "<< test5.at(i).messageInfo.UserMessage<<"Dataset: "<<test5.at(i).faceInfo.PathDataset<<"Finger: "<<test5.at(i).fingerInfo.FingerprintName<<endl;
   }

   if(ola.deleteFullUser("ANDRE")) cout<<"Deleted with success!!\n"; else cout <<"error deleting\n!!";

   test5=ola.getAllUsers();

   for(unsigned int i=0 ; i< test5.size();i++)
   {
       cout<<"USER "<<i<<": "<<test5.at(i).user.NickName<<"Message: "<< test5.at(i).messageInfo.UserMessage<<"Dataset: "<<test5.at(i).faceInfo.PathDataset<<"Finger: "<<test5.at(i).fingerInfo.FingerprintName<<endl;
   }

   if(ola.updateNumberOfImage("PAULO",20))
   {
       ola.getFace("PAULO",&test1);
       cout<<" NEW Face Parsing:"<<test1.PathDataset<<","<<test1.NumberOfImages<<endl;

   }
   if(ola.updateJumpTime("PAULO",5000))
   {
       ola.getMessage("PAULO",&test3);
       cout<<"NEW Message parsing"<<test3.JumpTime<<endl;
   }
   if(ola.updateUserMessage("PAULO","Olaa afinal ja esta mais quente!!"))
   {
       ola.getMessage("PAULO",&test3);
       cout<<"NEW Message2 parsing"<<test3.UserMessage<<endl;
   }
   if(ola.updatePlatformToRelease("PAULO","EMAIL andrecampos@hotmail.com"))
   {
       ola.getMessage("PAULO",&test3);
       cout<<"NEW Message3 parsing"<<test3.TargetPlatform<<endl;
   }


   return 0;
}
