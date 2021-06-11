/* @saglammust
* Student Name: Mustafa Saðlam
* Student ID : 150140129
* Date: 17.10.16
*/ 

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iomanip>

#define NUMOFPATIENT 100

using namespace std;

struct patient_record {
    char name[20], doctorName[20], diagnosis[20];      
	int patientNumber, polyclinicNumber;  
	char branchName[20];
}; 

struct clinic_info { // for constraints while inserting a new patient
	char name_branch[20]; // storing branchName of an interval
	int patient_count; // storing how many patient enrolled in a polyclinic
	int clinic_num; // storing polyclinicNumber of an interval
};
/* by using this struct we know that ;
	*which clinic belongs to which branch
	*how many clinic owned by a branch
	*and how nany patients enrolled in a clinic*/

struct datastruct { //there is not an insert fuction here because with parameters, constraints can not be satisfied 'easily'
	void begin();//
	void print_all();//
	void remove(int patient_num);//
	void search_branch(char target[]);//
	void search_polyclinic(int target);//
	void readfrom();//
	void writeto();
	patient_record recordbook[NUMOFPATIENT];
	clinic_info clinicbook[NUMOFPATIENT / 10];
	FILE *dataP;
	int ordernum, recordnum;
};

void datastruct::readfrom(){
	//opening_file
	dataP = fopen("database.txt","r");
	if(!(dataP)){
		if(!(dataP = fopen("database.txt","w"))){
			cerr << "Sorry, the file could not open!..." << endl;
			exit(1);
		}
	}//end-opening_file
	
	//reading (orderless)
	char ch; int count = 0;
	for (int i = 0; i<100; i++){	
		while(ch != '\t');{
			ch = (char)(fgetc(dataP));
			recordbook[i].name[count] = ch;
			count++;
			if(count == 20) break;
		}count = 0;
		
		while(ch!= '\t'){
			ch = (char)(fgetc(dataP));
			recordbook[i].doctorName[count] = ch;
			count++;
			if(count == 20) break;
		}count = 0;
		
		while(ch != '\t'){
			ch = (char)(getc(dataP));
			recordbook[i].diagnosis[count] = ch;
			count++;
			if(count == 20) break;
		}count = 0;
		
		char store_patient_num[5];
		while(ch != '\t'){
			ch = (char)(fgetc(dataP));
			store_patient_num[count] = ch;
			recordbook[i].patientNumber = atoi(store_patient_num);
			count++;
			if(count == 20) break;
		}count = 0;
		
		char store_clinic_num[3];
		while(ch != '\t'){
			ch = (char)(fgetc(dataP));
			store_clinic_num[count] = ch;
			recordbook[i].polyclinicNumber = atoi(store_clinic_num);
			count++;
			if(count == 20) break;
		}count = 0;
		
		while(ch != '\n'){
			ch = (char)(fgetc(dataP));
			recordbook[i].branchName[count] = ch;
			count++;
			if(count == 20) break;
		}count = 0;
	/* 
		fscanf(dataP, "%[^\t]", recordbook[i].name);
		fscanf(dataP, "%[^\t]", recordbook[i].doctorName);
		fscanf(dataP, "%[^\t]", recordbook[i].diagnosis);
		fscanf(dataP, "%[^\t]", &recordbook[i].patientNumber);
		fscanf(dataP, "%[^\t]", &recordbook[i].polyclinicNumber);
		fscanf(dataP, "%[^\n]", recordbook[i].branchName);
*/
		recordnum++;
	}//end-reading
	print_all();

	//ordering A (alphabetical by branchName and polyclinicNumber)
	patient_record temp_patient;
	for (int i = 0; i < recordnum; i++){
		for (int j = i + 1; j < recordnum; i++){
			if (strcmp(recordbook[i].branchName,recordbook[j].branchName) > 0){
				temp_patient = recordbook[i];
				recordbook[i] = recordbook[j];
				recordbook[j] = temp_patient;
			}
			else if (strcmp(recordbook[i].branchName,recordbook[j].branchName) == 0){
				if (recordbook[i].polyclinicNumber > recordbook[j].polyclinicNumber){
					temp_patient = recordbook[i];
					recordbook[i] = recordbook[j];
					recordbook[j] = temp_patient;
				}
			}
		}
	}//end-ordering A

	//assigning (the first values of clinicbook)
	int i = 1, temp_num1 = 0, temp_num2 = 0;//start with i=1 to compare with i=0 ... temp_num1 is an increment variable for clinicbook
											//temp_num2 is keeping i's previous value to calculate how many people in the new interval
	while (i < recordnum){
		if (recordbook[i].polyclinicNumber != recordbook[i - 1].polyclinicNumber){//if polyclinicNumber changes in records ordered above
			clinicbook[temp_num1].patient_count = i - temp_num2;//number of patients in that interval
			strcpy(clinicbook[temp_num1].name_branch, recordbook[i - 1].branchName);//branchName of that interval
			clinicbook[temp_num1].clinic_num = recordbook[i - 1].polyclinicNumber;//polyclinicNumber of taht interval
			temp_num2 = i;
			temp_num1++;
		}
		if (recordbook[i].branchName[0] == '\0'){
			for (int j = NUMOFPATIENT / 10; temp_num1 < j; j--){
				clinicbook[j].patient_count = 0;//this is used by insert function to determine whether the clinic exists or not
			}
			break;
		}
	i++;
	}//end-assigning

	//copying (for struct ordering in groups of ten patient)
	patient_record temprecordbook[NUMOFPATIENT];
	for (int i = 0; i < NUMOFPATIENT; i++){
		temprecordbook[i] = recordbook[i];
	}//end-copying

	//ordering B (in groups of ten)
	int skipper, numerator = 0;
	for (int i = 0; i < NUMOFPATIENT / 10; i++){
		skipper = i * 10;
		while (skipper < (i * 10) + clinicbook[i].patient_count){//exaple: from 0 to 7 or from 10 to 14 or from 20 to 26
			recordbook[skipper] = temprecordbook[numerator];
			numerator++;// counts how many assigning operations have done which is equal to element number of the next patient in temprecordbook
		}
	}//end-ordering B
	fclose(dataP);
}//endF-readfrom

void datastruct::begin(){
	ordernum = 1;
	recordnum = 0;	
	
}//endF-begin

void datastruct::search_polyclinic (int target){
	for(int i=0; i<NUMOFPATIENT; i++){
		if (target == recordbook[i].polyclinicNumber){
			cout << ordernum << ". " << recordbook[i].name << "\t" << recordbook[i].doctorName << "\t" << recordbook[i].diagnosis << "\t";
			cout << recordbook[i].patientNumber << "\t\t" << recordbook[i].polyclinicNumber << "\t\t" << recordbook[i].branchName << endl;
		}
		ordernum++;
	}
	ordernum = 1; // set to zero(actually 1) to use it again since it is a struct variable in datastruct
}//endF-search_polyclinic

void datastruct::search_branch(char target[]){
	for(int i = 0; i < NUMOFPATIENT; i++){
		if (strnicmp(recordbook[i].branchName, target, strlen(target)) == 0){
			cout << ordernum << ". " << recordbook[i].name << "\t" << recordbook[i].doctorName << "\t" << recordbook[i].diagnosis << "\t";
			cout << recordbook[i].patientNumber << "\t\t" << recordbook[i].polyclinicNumber << "\t\t" << recordbook[i].branchName << endl;
		}
		ordernum++;
	}
	ordernum = 1;
}//endF-search_branch

void datastruct::print_all(){
	int skipper, numerator = 1;//this time it is counting the printed elements 
	for (int i = 0; i < NUMOFPATIENT / 10; i++){
		skipper = i * 10;
		while (skipper < (i * 10 + clinicbook[i].patient_count)){
			cout << numerator << ". " << recordbook[skipper].name << "\t" << recordbook[skipper].doctorName << "\t";
			cout << recordbook[skipper].diagnosis << "\t" << recordbook[skipper].patientNumber << "\t\t";
			cout << recordbook[skipper].polyclinicNumber << "\t\t" << recordbook[skipper].branchName << endl;
		}
	}
}//endF-print_all

void datastruct::remove(int patient_num){
	//searching (for appropriate patient number since there is not a search func. which search records by patient number
	int keep;
	bool truthseeker = false;
	int control = 0;
	for (int i = 0; i < NUMOFPATIENT; i++){
		if (patient_num == recordbook[i].patientNumber){
			cout << ordernum << ". " << recordbook[i].name << "\t" << recordbook[i].doctorName << "\t" << recordbook[i].diagnosis << "\t";
			cout << recordbook[i].patientNumber << "\t\t" << recordbook[i].polyclinicNumber << "\t\t" << recordbook[i].branchName << endl;
			cout << "\nAre you sure that you want to delete this record?" << endl;
			cout << "If you sure please press 1. If you not, press any key to exit without deletion :";
			cin >> control;
			keep = i;
			truthseeker = true;
			break;
		}
	}
	if (truthseeker == false)
		cerr << "Sorry, the number you are looking to delete could not find..." << endl;
	//end-searching
	
	if (control == 1){//deleting (permanently)
		int interval = keep / 10 - 1;//finding in which interval patient is
		int number_i = keep % 10 + 1;//finding what is the order of the number in interval
		for (int i = number_i; number_i < (interval + 1) * 10; number_i++){
			recordbook[number_i] = recordbook[number_i - 1];//by sliding records in the interval the record which was wanted to delete is deleted
		}
	}//end-deleting
}//endF-remove

void datastruct::writeto(){
	dataP = fopen("database.txt", "w+");//opening in writing mode
	for (int i = 0; i < NUMOFPATIENT / 10; i++){
		for (int j = 0; j < clinicbook[i].patient_count; j++){//only writes real records not blank ones
			fprintf(dataP, "%s\t%s\t%s\t", recordbook[i * 10 + j].name, recordbook[i * 10 + j].doctorName, recordbook[i * 10 + j].diagnosis);
			fprintf(dataP, "%d\t%d\t%s\n", recordbook[i * 10 + j].patientNumber, recordbook[i * 10 + j].polyclinicNumber, recordbook[i * 10 + j].branchName);
		}
	}
	fclose(dataP);
}//endF-writeto
