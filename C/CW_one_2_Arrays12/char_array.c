#include<stdio.h>
void main()
{
    char str[]={'M','e','m','o','n',' ','A','b','d','u','l',' ','Q','a','d','i','r',' ','G','u','l','a','m',' ','M','o','h','i','y','u','d','d','i','n','\0'};
    //Bekaar Tarika

    /*int m=sizeof(str)/sizeof(str[0]);
    printf("%d\n", m);
    for(int i=0;i<m;i++){
        printf("%c",str[i]);
    }*/

    printf("%s", str);
    //Lekin lekin if int str[] ke baad %s se printing nahi hogi.
    printf("\n");
    //Declaration of string
    //Uper wala int se string chal gaya par "" iske ander daal ne ke baad nahi chala.
    char str1[]="Memon Abdul \0 Qadir Gulam Mohiyuddin";

    /*int n=sizeof(str1)/sizeof(str1[0]);
    printf("%d\n", n);
    for(int i=0;i<n;i++){
        printf("%c",str1[i]);
    }*/

    printf("%s", str1);
    //Why size differ? Due to different declaration.
    char str2[20]="Memon Abdul Qadir Gulam Mohiyuddin";
    printf("%s", str2);
}
