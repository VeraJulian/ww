/*                                                                              
 * (c) 2015 Julian Vera                                                         
 * This program reports duplicate adjacent words in text. A word is a           
 * string of alphabetic characters, delimited by non-alphabetic characters      
 * or file boundaries. As an argument this program takes the name of            
 * zero or more files. When duplicates are found, the program prints the        
 * filename, line of the first instance, and the lowercase word all seper-      
 * ated by colons. It is insensitive to case and leaves with non-0 exit         
 * status only when pairs are found                                             
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

/*                                                                              
 * This method is the meat of the program. It reads in the characters,          
 * stores them in an array and makes the comparison to check for adjacent       
 * duplicates                                                                   
 */
void printWords(FILE * fp, char* name)
{
  //Variable used to assign some space to pointer variables                     
  int size = 4;

  //These are pointers to previous and current word                             
  char* cur = malloc(size*sizeof(char));
  char* previous = malloc(sizeof(char));

  //length of cur word                                                          
  int i = 0;

  //variable used to read characters (letter in this case)                      
  int c;

  //initially there are no lines                                                
  int line_num = 0;

  //While there is something to read, or check                                  
  while(1){
    i = 0;
    //only concerned with letters and adjacent words                            
    while(isalpha(c = fgetc(fp))){
      //determine if current word has reached the char array limit              
      if(i >= size) cur = realloc(cur,sizeof(char)*(size*=2));
      //if not, then placed lowercased letters into cur char array              
      cur[i++] = tolower(c);
    }
    //end the array with 0, since it's holding chars                            
    cur[i] = 0;

    //if the previous and current words are equal and cur is greater than 0     
    //print the name, line number and word                                      
    if( (!strcasecmp(cur,previous)) && i > 0){
      printf("%s:%d:%s\n",name,line_num,cur);
    }
    //if we reach EOF there is nothing left to read                             
    if(c == EOF) return;

    /*                                                                          
     * if we hit new line character then increase number of lines               
     * so we can later determine which line has the duplicated adjacent         
     * words                                                                    
     */
    if(c == '\n') line_num++;
    if(i){
      //free allocted memory for previous word since it'll be assigned new      
      //value                                                                   
      free(previous);
      //Assign current word to previous word since we're only concerned         
      //with adjacent words                                                     
      previous = cur;
      //allocate memory for next (cur) word                                     
      cur = malloc((size=4)*sizeof(char));
    }
  }
}

int main(int argc, char** argv)
{
  //Create a file pointer                                                       
  FILE* fp;

  //Check for file input                                                        
  if(argc > 1){

    //If there are multiple files read them all                                 
    while((--argc)>0){
      argv++;
      fp = fopen(argv[0],"r");

      //printWords will read the input and determine duplicate adjacent         
      //words in the files                                                      
      printWords(fp,argv[0]);
      fclose(fp);
    }
  }else{
    //If there are no files then read from stdin                                
    printWords(stdin,"stdin");
  }
  return 0;
}
