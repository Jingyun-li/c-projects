#include <stdio.h>
#include <math.h>

/* author: Jingyun Li    */
/* UID number: 117242874 */
/* Directory ID: jingyli */

/* function prototype */
int calculate_score(int score[], int weight[], 
double weighted_score[],int N, int X);
double calculate_numerical(double weighted_score[], int days_late[],
 int weight[], int N, int P);
double calculate_avg(int score[], int days_late[], int N, int P);
double calculate_std(int score[], int days_late[], int average, int N, int P);

/* the main function*/
int main(){

  /* integer variables that the program will process */
  int P=10, N=10, X=10;
  char W ='x';

  /* arrays that will store the input assignments data */
  int number[50] = {0};
  int score[50] = {0};
  int weight[50] = {0};
  int days_late[50] = {0};
  double weighted_score[50] = {0};

  /* double variables that will serve as the output of the program */
  double total_weight = 0;
  double numeric_score = 0;
  double average = 0;
  double standard_dev = 0;
  
  /* int variables that will serve as cursor in loops */  
  int i,j;

  /* the inputs will be scanned here */
  scanf(" %d %d %c", &P, &X, &W);
  scanf(" %d", &N);

  /* this loop scans the data and put them in arrays */
  for(i = 0; i < N; i++){
    scanf("%d, %d, %d, %d", &number[i], &score[i], &weight[i], &days_late[i]);
  }
  
  /* check the validity of the data input by adding up the */
  /* weight and see if it reaches 100 */
  for(i = 0; i < N; i++){
    total_weight += weight[i];
  }
  /* if the total weight does not equals 100, */
  /*  the program will run to an end */
  if(total_weight != 100){
    printf("ERROR: Invalid values provided.\n");	
  } else {

    /* call the functions that help calculate the grade */
    calculate_score(score, weight, weighted_score, N, X);
    numeric_score = calculate_numerical(weighted_score,
 days_late, weight, N, P);
    
    printf("Numeric Score: %5.4f\n", numeric_score); 
    printf("Points Penalty Per Day Late: %d\n", P);
    printf("Number of Assignments Dropped: %d\n", X);
    printf("Values Provided:\n");
    printf("Assignment, Score, Weight, Days Late\n");

    /* use a a nested loop to print the data in order */
    for(i = 1; i <= N; i++){
      for(j = 0; j < N; j++){
	if(number[j] == i){
	  printf("%d, %d, %d, %d\n", number[j],
	  score[j], weight[j], days_late[j]);
	  break;
	}
      }
    }
    
    /* print statistical information, if W equal to either ’Y’ or ’y’ */
    if(W == 'Y' || W == 'y'){
      average = calculate_avg(score, days_late, N, P);
      standard_dev = calculate_std(score, days_late, average, N, P);
      printf("Mean: %5.4f, Standard Deviation: %5.4f\n", average, standard_dev);
    }
  } 
  return 0;
}

/* this function calculate the weighted grade of all grades */
/* given, then turn the lowest drop some grades by turning */
/* them to 0 in the weighted_grade array */
int calculate_score(int score[], int weight[], 
double weighted_score[], int N, int X) {
  int i = 0;
  int j = X;
  double min = 100.00;
  /* this loop put the weighted socres into the array */
  for(i = 0; i < N; i++){
    weighted_score[i] = score[i] * weight[i] * 0.01;
  }

  /* this loop eliminate all the scores that should be dropped */
  /* by changing teh data in the array to 0. */ 
  while(j != 0){
    for(i = 0; i < N; i++){
      if(weighted_score[i] < min && weighted_score[i]!=0){
	min = weighted_score[i];
      }
    }
    for(i = 0; i < N; i++){
      if(weighted_score[i] == min){
	weighted_score[i] = 0;
        j--;
	min = 100.00;
      }
    }
  }
  return 0;
}

/* this function calculates the final numerical score. */
double calculate_numerical(double weighted_score[], int days_late[], 
int weight[], int N, int P){
  int i = 0;  
  double numerical_score = 0;
  double total_weight = 0;
  /* this is a loop that add all the deducted weighted scores */
  /* and calculates the total weight */
  for(i = 0; i < N; i++) { 
    if(weighted_score[i] != 0){
      numerical_score += weighted_score[i]-
	(0.01 * P * weight[i] * days_late[i]);
      total_weight += weight[i];
    }
  }
  /* the numerical score is calculates by dividing */
  /* the total numerical score by the weight */
  numerical_score = numerical_score/(0.01 * total_weight);
  return numerical_score;
}

/* this function calculates the average */
double calculate_avg(int score[], int days_late[],int N, int P){
  double total = 0;
  double average = 0;
  int i = 0;
  /* this loop add up all the scores after deducting the late panelty */
  for(i = 0; i < N; i++){
    total += score[i] - (days_late[i] * P);
  }
  average = total/N;
  return average;
}

/* this function calculates the standard deviation */
double calculate_std(int score[], int days_late[], int average, int N, int P){
  int i = 0;
  double variance = 0;
  double standard_dev = 0;

  /* this loop calculates variance */
  for(i = 0; i < N; i++){
    variance += (score[i] - (days_late[i] * P) - average)*
      (score[i] - (days_late[i] * P) - average);
  }

  standard_dev = sqrt(variance/N);
  return standard_dev;
}
