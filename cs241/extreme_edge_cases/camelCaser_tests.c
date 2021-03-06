/**
 * MP: Extreme Edge Cases
 * CS 241 - Fall 2017
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "camelCaser.h"
#include "camelCaser_tests.h"

/*
 * Testing function for various implementations of camelCaser.
 *
 * @param  camelCaser   A pointer to the target camelCaser function.
 * @param  destroy      A pointer to the function that destroys camelCaser
 * output.
 * @return              Correctness of the program (0 for wrong, 1 for correct).
 */
int test_camelCaser(char **(*camelCaser)(const char *),
                    void (*destroy)(char **)) {
    // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
    
    
  	char** test = camelCaser(NULL); // NULL TEST
	if (test != 0) return 0;
  	destroy(test);
  	printf("test NULL in passed.\n");    
 
    char t1[] = "      wor \v \ak, work"; // incomplete sentence test
  	test = camelCaser(t1);
  	if (strcmp(test[0],"wor\aK") != 0) return 0;
  	if (test[1] != NULL) return 0;
  	//if (sizeof(test) != (sizeof(char*)*2)) return 0;
  	destroy(test);
  	printf("test ??? passed.\n");
  	
  	char t2[] = "  8csSXx mark 4know    ' sfasfsajkk"; //space test
  	test = camelCaser(t2);
  	if (strcmp(test[0],"8cssxxMark4Know") != 0) return 0;
  	if (test[1] != NULL) return 0;
 // 	if (sizeof(test) != sizeof(char*)*3) return 0;
 	//printf("sizeoftest= %lu, sizeofchar*= %lu\n",sizeof(test),sizeof(char*));
  	destroy(test);
  	printf("test incomplete + space passed.\n");
  	
  	char t3[] = "\b\bSSSS\nHHHHELL\nsdggSSSGDSAG sdkgjkl ,c,vdsd ";  // I dont know test
  	test = camelCaser(t3);
  	if (strcmp(test[0],"\b\bssssHhhhellSdggsssgdsagSdkgjkl") != 0) return 0;
  	if (strcmp(test[1],"c") != 0) return 0;
  	if (test[2] != NULL) return 0;
  	//if (sizeof(test) != sizeof(char*)*3) return 0;
  	destroy(test);
  	printf("test I dont know test passed.\n");
  	

  	
  	char t5[] = "12\v3 LE\\T M\bE .  d12se\nm gu\ni? we11\fis|me w\to me\b\bi nv\apy  & rty tryf k\bx+ ";
  	test = camelCaser(t5);
  	if (strcmp(test[0],"123Le") != 0) return 0;
  	if (strcmp(test[1],"tM\be") != 0) return 0;
  	if (strcmp(test[2],"d12seMGuI") != 0) return 0;
  	if (strcmp(test[3],"we11Is") != 0) return 0;
  	if (strcmp(test[4],"meWOMe\b\biNv\apy") != 0) return 0;
  	if (strcmp(test[5],"rtyTryfK\bx") != 0) return 0;
  	if (test[6] != NULL) return 0;
  	
  	//if (sizeof(test) != sizeof(char*)*2) return 0;
  	destroy(test);
  	printf("test5 passed.\n");
  	
  	char t6[] = " 2 cs 4fiskx;"; 
  	test = camelCaser(t6);
  	if (strcmp(test[0],"2Cs4Fiskx") != 0) return 0;
  	if (test[1] != 0) return 0;
  	//if (sizeof(test) != sizeof(char*)*3) return 0;
  	destroy(test);
  	printf("test num+word passed.\n");
  	
  	 
    char t7[] = "   \n wo hai shi zuixi HUan ASUka ya  ASUKA CHIANG SAI gao\t  ";
  	test = camelCaser(t7);
  	if (test[0] != 0) return 0;
  	//if (sizeof(test) != sizeof(char*)) return 0;
  	destroy(test);
  	printf("test7 passed.\n");
  	
  	char t8[] = ""; // empty string test
  	test = camelCaser(t8);
  	if (test[0] != 0) return 0;
  //	if (sizeof(test) != sizeof(char*)) return 0;
  	destroy(test);
  	printf("test empty string passed.\n");
  	
  	char t9[] = " ~ ! @  wyhj\\jj$ w/x%(>/?) s\a s^&*_-+=\\\t\b| BUU\buy / .";
  	test = camelCaser(t9);
  	if (*test[0] + *test[1] + *test[2] != 0) return 0;
  	if (strcmp(test[3],"wyhj") != 0) return 0;
  	if (strcmp(test[4],"jj") != 0) return 0;
  	if (strcmp(test[5],"w") != 0) return 0;
  	if (strcmp(test[6],"x") != 0) return 0;
  	if (*test[7] + *test[8] + *test[9]+ *test[10] + *test[11] != 0) return 0;
  	if (strcmp(test[12],"s\aS") != 0) return 0;
  	int ii = 13;
  	for (; ii <= 19; ii++) {
  		if (*test[ii] != 0) return 0;
  	}
  	if (strcmp(test[20],"\b") != 0) return 0;
  	if (strcmp(test[21],"buu\buy") != 0) return 0;
  	if (*test[22] != 0) return 0;
  	if (test[23] != NULL) return 0;
 // 	if (sizeof(test) != sizeof(char*)*5) return 0;
  	destroy(test);
  	printf("test puncs passed.\n");
  	
  	char t10[] = "       ";
  	test = camelCaser(t10);
  	if (test[0] != NULL) return 0;
  	destroy(test);
  	printf("test white passed.\n");
  	
	char tnum[] = "   \n\t1a9  nu\\mxix CN\tGV651KX\\9ytear8\t0  2xxx12?";
	test = camelCaser(tnum);
	if (strcmp(test[0],"1a9Nu") != 0) return 0;
	if (strcmp(test[1],"mxixCnGv651kx") != 0) return 0;
	if (strcmp(test[2],"9ytear802Xxx12") != 0) return 0;
	if (test[3] != NULL) return 0;
	destroy(test);
	printf("testnum passed.\n");
	

	
	char avcc[] = "\0TESTNULL??\0\0hahaha.\0";
	test = camelCaser(avcc);
	if (test[0] != NULL) return 0;
	destroy(test);
	printf("white\n");
	
	
	char stress[] = "Stress test test sfffccc yesy t\\ets xxxx gllgs%zzx glls ggs^fkx ksgjzalfmf lll zx,vm   FFFfl^gsdxxx SSssx xx;a;faaa sarkk  !safaj fsasfas!~ test zh\vdsaeng kkxx sdkk \\ sagjks ! zckas sdgk \tzzz;";
	test = camelCaser(stress);
	if (strcmp(test[0],"stressTestTestSfffcccYesyT") != 0) return 0;
	if (strcmp(test[1],"etsXxxxGllgs") != 0) return 0;
	if (strcmp(test[2],"zzxGllsGgs") != 0) return 0;
	if (strcmp(test[3],"fkxKsgjzalfmfLllZx") != 0) return 0;	
	if (strcmp(test[4],"vmFfffl") != 0) return 0;		
	if (strcmp(test[5],"gsdxxxSsssxXx") != 0) return 0;		
  	if (strcmp(test[6],"a") != 0) return 0;	
  	if (strcmp(test[7],"faaaSarkk") != 0) return 0;	
  	if (strcmp(test[8],"safajFsasfas") != 0) return 0;	
  	if (*test[9] != 0) return 0;
  	if (strcmp(test[10],"testZhDsaengKkxxSdkk") != 0) return 0;
	if (strcmp(test[11],"sagjks") != 0) return 0;
	if (strcmp(test[12],"zckasSdgkZzz") != 0) return 0;
	if (test[13] != NULL) return 0;
	destroy(test);
    printf("stress!!\n");
    return 1;
}
