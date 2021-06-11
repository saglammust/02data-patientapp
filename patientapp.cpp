/* @saglammust
* Student Name: Mustafa Saðlam
* Student ID : 150140129
* Date: 17.10.16
*/ 

#include"record_op.h"

typedef datastruct databook;
databook book;

void menu_print();//
int operation_decider(char choice);//
void search_record_by_branch();//
void search_record_by_clinic();//
void insert_record();// 
void remove_record();//

int main(){
	book.begin();
	book.readfrom(); 
	int end_check = 0;
	char choice;
	while (end_check <= 0){
		menu_print();
		cin >> choice;
		end_check = operation_decider(choice);
	}
	return 0;
}//endF-main*
 
void menu_print(){
	//system("cls");
	cout << endl;
	cout << "Patient-Hospital Record Application" << endl;
	cout << "Please select the operation to perform and enter the operation code" << endl;
	cout << "(P) 'P'rint all of the patient records," << endl;
	cout << "(B) Search the data by the 'b'ranch name," << endl;
	cout << "(C) Search the data by the poly'c'linic number," << endl;
	cout << "(I) 'I'nsert a new patient record" << endl;	
	cout << "(R) 'R'emove the patient record" << endl;
	cout << "(E) 'E'xit the program" << endl;	
	cout << endl;
	cout << "Your selection is: ";	
}//endF-print_menu()

int operation_decider(char choice){
	int end=0;
	switch (choice) { 
	case 'P': case 'p': 
		book.print_all();
		break; 
	case 'B': case 'b': 
		search_record_by_branch();
		break; 
	case 'C': case 'c': 
		search_record_by_clinic();
		break;
	case 'I': case 'i': case 'ý': //for Turkish keyboards
		insert_record();
		break;
	case 'R': case 'r': 
		remove_record();
		break; 
	case 'E': case 'e': 
		cout << "Are you sure you want to exit the program? (Y/N):";
		cin >> choice;
		if ( choice == 'Y' || choice == 'y' )
			book.writeto();
			end = 1; 
			break; 
	default: 
		cout << "Error: You have entered an invalid choice" << endl; 
		cout << "Please try again [ P, B, C, I, R, E ] :" ;
		cin >> choice;
		end = operation_decider(choice);
		break; 
	}
	return end;
}//endF-operation_decider()

void insert_record(){// all about inserting a new patient is here
	patient_record newpatient;
	cout << "Please enter the specified information about patient below" << endl;
	cin.ignore(1000, '\n');

	cout << "Patient's Name: ";
	cin.getline(newpatient.name,20,'\n');
	
	cout << "Doctor's Name: ";
	cin.getline(newpatient.doctorName,20,'\n');
	
	cout << "Diagnosis of Patient: ";
	cin.getline(newpatient.diagnosis,20,'\n');	
	
	cout << "Doctor's Branch: ";
	cin.getline(newpatient.branchName,20,'\n');	
	
	cout << "Polyclinic Number: ";
	cin >> newpatient.polyclinicNumber;
	
	do{
		cout << "Patient Number (4 digit): ";
		cin >> newpatient.patientNumber;
	}while(!((newpatient.patientNumber > 999) && (newpatient.patientNumber <= 9999)));//to make patient number 4 digit

	//finding_appropriate_place (for the new record)
	//since every polyclinic has only one branch if in which interval is newpatient.polyclinicNumber is known 
	//it can be easily placed by clinic_info clinicbook structure
	
	int clinic_counter = 0;
	int i;
	for (i = 0; i < NUMOFPATIENT / 10; i++){
		if ((strcmp(book.clinicbook[i].name_branch, newpatient.branchName) == 0) && (book.clinicbook[i].clinic_num != newpatient.polyclinicNumber)){
			clinic_counter++;//counts the records which have the same branch with the new record but not the same polyclinic number
			if (clinic_counter > 1){//means if there is more than one unappropriate branchName-polyclinicNumber couple then this situation
									//constrains the rule "each branch can serve at most 2 polyclinic" 
				cerr << "Sorry, this branch(" << book.clinicbook[i].name_branch << ") is already have '2' polyclinics." << endl;
				cerr << "Please try again and enter the proper polyclinic number or branch..." << endl;
				break;
			}
		}
		if (book.clinicbook[i].clinic_num == newpatient.polyclinicNumber){
			if (strcmp(book.clinicbook[i].name_branch, newpatient.branchName) != 0){//only one branch can serve in a polyclinic
				cerr << "Sorry, this polyclinic is serving for '" << book.clinicbook[i].name_branch << "' branch;" << endl;
				cerr << "Please try another polyclinic!..." << endl;
				break;
			}

			if (book.clinicbook[i].patient_count == 10){//at most 10 patient for one polyclinic
				cerr << "Sorry, this clinic already have 10 patient; \nPlease try another clinic!..." << endl;
				break;
			}
			else if (book.clinicbook[i].patient_count == 0){//creating new interval : new branch or clinic
				book.recordbook[i * 10 + book.clinicbook[i].patient_count] = newpatient;//skipped to i'th interval then add how many people in that interval 
				book.clinicbook[i].patient_count++;
				book.clinicbook[i].clinic_num = newpatient.polyclinicNumber;
				strcpy(book.clinicbook[i].name_branch, book.recordbook[i].branchName);
				cout << "Records of the new patient have sucsessfully inserted..." << endl;
				break;
			}
			else if (book.clinicbook[i].patient_count < 10){
				book.recordbook[i * 10 + book.clinicbook[i].patient_count] = newpatient;//skipped to i'th interval then add how many people in that interval 
				book.clinicbook[i].patient_count++;
				cout << "Records of the new patient have sucsessfully inserted..." << endl;
				break;
			}
		}
	}
	cout << ".end." << endl;
	getchar();
}//endF-insert_record()

void search_record_by_branch(){
	char aim[10];
	cout << "Please, enter doctor's branch: ";
	cin.ignore(2000, '\n');
	cin.getline(aim, 10, '\n');
	book.search_branch(aim);
}//endF-search_record_by_branch()

void search_record_by_clinic(){ 
	int aim;
	cout << "Please, enter polyclinic number:";
	cin >> aim;
	book.search_polyclinic(aim);
}//endF-search_record_by_branch()

void remove_record(){
	int aim;
	cout << "Please enter the 4 digit patient number: ";
	cin >> aim;
	book.remove(aim);
}//endF-remove_record()
