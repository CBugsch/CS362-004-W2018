#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	//create the array of the test variables
	char testVars[9] = { '[', '(', '{', ' ', 'a', 'x', '}', ')', ']' };
	
	//get a random number [0 - 9]
	int testChar = rand() % 10;

	//return the char that coresponds to the index of the rand number
	return testVars[testChar];
}

char *inputString()
{
	int randNum;		//hold the random num
	char *testString;	//hold the random string
	
	//create an array of the test vars
	char testVars[5] = { 'r', 'e', 's', 't', 0 };

	//allocate space for the string
	testString = malloc(sizeof(char) * 6);

	//loop through and assign a random char from the testVars to the string
	for (int i = 0; i < 6; i++) {
		randNum = rand() % 5;
		testString[i] = testVars[randNum];
	}

	//return the string
	return testString;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
