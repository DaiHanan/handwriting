//#include <iostream>
//#include <winsock.h>
//#include <mysql.h>
//
//using namespace std;
//
//int main()
//{
//    const char host[] = "localhost";    //MySQL������IP��ַ�����Ǳ��ؿ���д��localhost����127.0.0.1  
//    const char user[] = "root";         //MySQL���û���  
//    const char pswd[] = "liyulin";         //����  
//    const char table[] = "test";        //���ݿ�����  
//    unsigned int port = 3306;           //MySQL����˿ںţ�Ĭ����3306          
//    MYSQL myCont;//����MYSQL���󣬶������ݿ����Ӿ��  
//    MYSQL_RES* result = NULL;//��ѯ���������Ų�ѯ���  
//    MYSQL_ROW sql_row;//���һ�в�ѯ������ַ�������  
//    MYSQL_FIELD* fd; // �����ֶ���Ϣ�Ľṹ
//    char column[32][32];
//    int res;
//    mysql_library_init(0, NULL, NULL);//��ʼ��MySQL��  
//    mysql_init(&myCont);//��ʼ�����Ӵ������  
//    if (mysql_real_connect(&myCont, host, user, pswd, table, port, NULL, 0))
//    {//ͨ������mysql_real_connect()���ӵ�������  
//        cout << "connect succeed!" << endl;
//        mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����  
//        res = mysql_query(&myCont, "select * from student");//ִ�в�ѯ��䣬mysql_query�����ѯ�ɹ����㣻�������һ�����󣬷��㡣  
//        if (!res)
//        {
//            result = mysql_store_result(&myCont);//�����ѯ�������ݵ�result  
//            if (result)
//            {
//                int i, j;
//                cout << "number of result: " << (unsigned long)mysql_num_rows(result) << endl;
//                for (i = 0; fd = mysql_fetch_field(result); i++)//��ȡ����  
//                {
//                    strcpy_s(column[i], fd->name);
//                }
//                j = mysql_num_fields(result);
//                for (i = 0; i < j; i++)
//                {
//                    printf("%s\t", column[i]);
//                }
//                printf("\n");
//                while (sql_row = mysql_fetch_row(result))//��ȡ���������  
//                {
//                    for (i = 0; i < j; i++)
//                    {
//                        printf("%s\n", sql_row[i]);
//                    }
//                    printf("\n");
//                }
//            }
//        }
//        else
//        {
//            cout << "query sql failed!" << endl;
//        }
//    }
//    else
//    {
//        cout << "connect failed!" << endl;
//    }
//    //ע���������ݿ�Ҫ��ʱ������Դ  
//    if (result != NULL) mysql_free_result(result);//�ͷŽ����Դ  
//    mysql_close(&myCont);//�ر�MySQL����  
//    mysql_library_end();//�ر�MySQL��  
//    return 0;
//}