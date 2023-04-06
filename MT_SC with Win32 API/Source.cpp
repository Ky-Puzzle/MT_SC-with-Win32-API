#include <Windows.h>
#include <stdio.h>
#include<iostream>
#include <iterator>
#include <map>
#include <string>
#include <stdlib.h>
using namespace std;


struct create_service_i {
	LPCSTR service_name;
	LPCSTR binary_path = "";
	LPCSTR display_name = NULL;
	string service_type_user = "own";
	string service_type_user2;
	string start_type_user = "demand";
	string error_control_user = "normal";
	LPCSTR group = NULL;
	LPDWORD tag = NULL;
	LPCSTR ServiceStartName = NULL;
	LPCSTR depends = NULL;
	LPCSTR passwd = NULL;
};
struct delete_service {
	LPCSTR service_name;
};
struct qdesc_service {
	LPCSTR service_name;
	DWORD bufsize = 1024;
};
struct starting__stopping_service {
	LPCSTR service_name;
};
struct querying_service {
	LPCSTR service_name;
	string type = "service";
	string service_type = "own";
	string state = "active";
	DWORD bufsize = 1024;
	int ri = 0;
	string group = "";
};
struct config_failure_service {
	LPCSTR service_name;
	DWORD reset_period = INFINITE;
	LPSTR reboot_msg = NULL;
	LPSTR  command = NULL;
	string actions;
};
struct config_service_i {
	LPCSTR service_name;
	LPCSTR binary_path = NULL;
	LPCSTR display_name = NULL;
	string service_type_user;
	string start_type_user;
	string service_type_user2;
	string error_control_user;
	LPCSTR group = NULL;
	LPDWORD tag = NULL;
	LPCSTR ServiceStartName = NULL;
	LPCSTR depends = NULL;
	LPCSTR passwd = NULL;
};

DWORD get_buffer_config_size_desc(LPCSTR service_name) {
	//Function serves to get the required buffer size for qdescription

	DWORD bytes_n, buffer_req;


	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	SC_HANDLE open_service_handler = OpenService(service_control_handler, service_name, SERVICE_QUERY_CONFIG);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}
	QueryServiceConfig2(open_service_handler, SERVICE_CONFIG_DESCRIPTION, NULL, 0, &buffer_req);
	bytes_n = buffer_req;

	CloseServiceHandle(open_service_handler);
	CloseServiceHandle(service_control_handler);
	return bytes_n;

}
DWORD get_buffer_config_size_fail(LPCSTR service_name) {
	//Function serves to get the required buffer size for failure

	DWORD bytes_n, buffer_req;


	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	SC_HANDLE open_service_handler = OpenService(service_control_handler, service_name, SERVICE_QUERY_CONFIG);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}
	QueryServiceConfig2(open_service_handler, SERVICE_CONFIG_FAILURE_ACTIONS, NULL, 0, &buffer_req);
	bytes_n = buffer_req;

	CloseServiceHandle(open_service_handler);
	CloseServiceHandle(service_control_handler);
	return bytes_n;

}
//Create Service
void create_my_service(create_service_i created_service) {

	DWORD service_type = NULL;


	//service type 
	map<string, DWORD> service_type_selection =
	{
		{"own",SERVICE_WIN32_OWN_PROCESS},
		{"share",SERVICE_WIN32_SHARE_PROCESS},
		{"kernel",SERVICE_KERNEL_DRIVER},
		{"filesys",SERVICE_FILE_SYSTEM_DRIVER},
		{"rec",SERVICE_RECOGNIZER_DRIVER}

	};
	map<string, DWORD>::iterator it;
	it = service_type_selection.find(created_service.service_type_user);

	if (it != service_type_selection.end()) {
		service_type = it->second;
	}
	else {
		printf("Service type selected is not an available option");
		exit(EXIT_FAILURE);
	};

	//start type
	DWORD start_type = NULL;
	map<string, DWORD> start_type_selection =
	{
		{"demand",SERVICE_DEMAND_START},
		{"boot",SERVICE_BOOT_START},
		{"system",SERVICE_SYSTEM_START},
		{"auto",SERVICE_AUTO_START},
		{"disabled",SERVICE_DISABLED}

	};
	map<string, DWORD>::iterator it1;
	it1 = start_type_selection.find(created_service.start_type_user);

	if (it1 != start_type_selection.end()) {
		start_type = it1->second;
	}
	else {
		printf("Start type selected is not an available option");
		exit(EXIT_FAILURE);
	};
	//error type
	DWORD error_control = NULL;
	map<string, DWORD> error_control_type_selection =
	{
		{"normal",SERVICE_ERROR_NORMAL},
		{"severe",SERVICE_ERROR_SEVERE},
		{"critical",SERVICE_ERROR_CRITICAL},
		{"ignore",SERVICE_ERROR_IGNORE}

	};
	map<string, DWORD>::iterator it2;
	it2 = error_control_type_selection.find(created_service.error_control_user);

	if (it2 != error_control_type_selection.end()) {
		error_control = it2->second;
	}
	else {
		printf("Error control selected is not an available option");
		exit(EXIT_FAILURE);
	};

	//Create the Service 
	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	//For interactive services
	if (created_service.service_type_user2 == "interact" && (created_service.service_type_user == "own" || created_service.service_type_user == "share")) {
		SC_HANDLE service_handler = CreateService(service_control_handler, created_service.service_name, created_service.display_name, SERVICE_ALL_ACCESS, service_type | SERVICE_INTERACTIVE_PROCESS, start_type, error_control, created_service.binary_path, created_service.group, created_service.tag, created_service.depends, created_service.ServiceStartName, created_service.passwd);

		if (service_handler == NULL) {
			printf("Failed to create service %x\n", GetLastError());
			exit(EXIT_FAILURE);
		}
		else {
			printf("Service created");
			return;
		}
	}
	//For everything else
	SC_HANDLE service_handler = CreateService(service_control_handler, created_service.service_name, created_service.display_name, SERVICE_ALL_ACCESS, service_type, start_type, error_control, created_service.binary_path, created_service.group, created_service.tag, created_service.depends, created_service.ServiceStartName, created_service.passwd);

	if (service_handler == NULL) {
		printf("Failed to create service %x\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	else {
		printf("Service created");
		return;
	}
}




//Delete Service
void delete_my_service(delete_service k) {

	//Getting Service Control Manager handle
	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}


	//opening service
	SC_HANDLE open_service_handler = OpenService(service_control_handler, k.service_name, SERVICE_ALL_ACCESS);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}
	//Deleting Service
	int deleted_service = DeleteService(open_service_handler);

	if (deleted_service == 0) {
		printf("Failed to delete service");
		exit(EXIT_FAILURE);
	}
	else {
		printf("Service has been deleted");
		return;
	}

}

//Start Service
void start_my_service(starting__stopping_service start_the_service) {
	//Getting Service Control Manager handle
	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	//opening service
	SC_HANDLE open_service_handler = OpenService(service_control_handler, start_the_service.service_name, SERVICE_ALL_ACCESS);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}
	int start_service = StartService(open_service_handler, 0, NULL);

	if (start_service == 0) {
		printf("Failed to start service");
		exit(EXIT_FAILURE);
	}
	else {
		printf("Service started");
		return;
	}

}

//Stop Service
void stop_my_service(starting__stopping_service stopping_the_service) {
	SERVICE_STATUS service_status;

	//Getting Service Control Manager handle
	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	//opening service
	SC_HANDLE open_service_handler = OpenService(service_control_handler, stopping_the_service.service_name, SERVICE_ALL_ACCESS);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	if (ControlService(open_service_handler, SERVICE_CONTROL_STOP, &service_status)) {
		printf("Service Stopped!");
	}

	else {
		printf("Failed to stop service");
		exit(EXIT_FAILURE);
	}

}
//To Store Current Failure Configuration 
struct configFailureInfoCurrent {
	LPCSTR service_name;
	DWORD     reset_period;
	LPSTR     reboot_msg;
	LPSTR     command;
	DWORD     c_actions;
	SC_ACTION* sa_Actions;
	DWORD bytesNeed;
} get_config_info_failure;

void get_current_config_failure(LPCSTR service_name) {
	DWORD bytes_req = get_buffer_config_size_fail(get_config_info_failure.service_name);

	DWORD buffer_req;
	SERVICE_FAILURE_ACTIONS* actions;
	actions = (SERVICE_FAILURE_ACTIONS*)malloc(bytes_req);


	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);

	}

	SC_HANDLE open_service_handler = OpenService(service_control_handler, service_name, SERVICE_QUERY_CONFIG);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}


	QueryServiceConfig2(open_service_handler, SERVICE_CONFIG_FAILURE_ACTIONS, (LPBYTE)actions, bytes_req, &buffer_req);

	get_config_info_failure.c_actions = actions->cActions;
	get_config_info_failure.reset_period = actions->dwResetPeriod;
	get_config_info_failure.reboot_msg = actions->lpRebootMsg;
	get_config_info_failure.command = actions->lpCommand;
	get_config_info_failure.sa_Actions = actions->lpsaActions;


	CloseServiceHandle(open_service_handler);
	CloseServiceHandle(service_control_handler);

}

//Failure Service
void failure_my_service(config_failure_service config_fail_serv) {
	//current failure config
	get_config_info_failure.service_name = config_fail_serv.service_name;
	get_current_config_failure(config_fail_serv.service_name);

	//service_failure_actions structure stuff
	SERVICE_FAILURE_ACTIONS serv_config_fail_actions;
	SC_ACTION lpsa_actions[4];

	//saving current config to new structure
	serv_config_fail_actions.cActions = get_config_info_failure.c_actions;
	serv_config_fail_actions.dwResetPeriod = get_config_info_failure.reset_period;
	serv_config_fail_actions.lpCommand = get_config_info_failure.command;
	serv_config_fail_actions.lpRebootMsg = get_config_info_failure.reboot_msg;
	serv_config_fail_actions.lpsaActions = get_config_info_failure.sa_Actions;


	//Checking and setting any changed values
	if (serv_config_fail_actions.dwResetPeriod != config_fail_serv.reset_period) {
		serv_config_fail_actions.dwResetPeriod = config_fail_serv.reset_period;
	}
	if (serv_config_fail_actions.lpCommand != config_fail_serv.command && config_fail_serv.command != NULL) {
		serv_config_fail_actions.lpCommand = config_fail_serv.command;
	}
	if (serv_config_fail_actions.lpRebootMsg != config_fail_serv.reboot_msg) {
		serv_config_fail_actions.lpRebootMsg = config_fail_serv.reboot_msg;
	}

	//setting any available actions
	if (config_fail_serv.actions.length() != 0) {
		map<string, SC_ACTION_TYPE> failure_action_selection =
		{
			{"",SC_ACTION_NONE},
			{"reboot",SC_ACTION_REBOOT},
			{"restart",SC_ACTION_RESTART},
			{"run",SC_ACTION_RUN_COMMAND},

		};
		map<string, SC_ACTION_TYPE>::iterator it;

		string token;
		size_t position = 0;
		string seperator = "/";

		DWORD tracker = 0;
		while ((position = config_fail_serv.actions.find(seperator)) != string::npos) {
			token = config_fail_serv.actions.substr(0, position);
			it = failure_action_selection.find(token);
			if (it != failure_action_selection.end()) {
				//cout << it->second << endl;
				lpsa_actions[tracker].Type = it->second;
			}
			else {
				DWORD conv_token = strtol(token.c_str(), 0, 0);
				lpsa_actions[tracker].Delay = conv_token;
				tracker++;
			}
			config_fail_serv.actions.erase(0, position + seperator.length());
		}
		token = config_fail_serv.actions.substr(0, position);
		DWORD conv_token = strtol(token.c_str(), 0, 0);
		lpsa_actions[tracker].Delay = conv_token;
		serv_config_fail_actions.cActions = tracker + 1;

	}



	serv_config_fail_actions.lpsaActions = lpsa_actions;


	//Updating the failure config 
	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	SC_HANDLE open_service_handler = OpenService(service_control_handler, config_fail_serv.service_name, SERVICE_ALL_ACCESS);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	if (!ChangeServiceConfig2(open_service_handler, SERVICE_CONFIG_FAILURE_ACTIONS, &serv_config_fail_actions)) {
		printf("Failure configuration failure %x\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	else {
		printf("Failure configuration success");
		return;
	}

}



//Configure Service
void configure_my_service(config_service_i configured_service) {

	//service type 
	DWORD service_type = NULL;
	if (configured_service.service_type_user != "") {
		map<string, DWORD> service_type_selection =
		{
			{"own",SERVICE_WIN32_OWN_PROCESS},
			{"share",SERVICE_WIN32_SHARE_PROCESS},
			{"kernel",SERVICE_KERNEL_DRIVER},
			{"filesys",SERVICE_FILE_SYSTEM_DRIVER},
			{"rec",SERVICE_RECOGNIZER_DRIVER}

		};
		map<string, DWORD>::iterator it;
		it = service_type_selection.find(configured_service.service_type_user);

		if (it != service_type_selection.end()) {
			service_type = it->second;
		}
		else {
			printf("Service type option not available. Service type will remain unchanged");
			service_type = SERVICE_NO_CHANGE;
		};
	}
	else {
		service_type = SERVICE_NO_CHANGE;
	}

	//start type
	DWORD start_type = NULL;
	if (configured_service.start_type_user != "") {
		map<string, DWORD> start_type_selection =
		{
			{"demand",SERVICE_DEMAND_START},
			{"boot",SERVICE_BOOT_START},
			{"system",SERVICE_SYSTEM_START},
			{"auto",SERVICE_AUTO_START},
			{"disabled",SERVICE_DISABLED}

		};
		map<string, DWORD>::iterator it1;
		it1 = start_type_selection.find(configured_service.start_type_user);

		if (it1 != start_type_selection.end()) {
			start_type = it1->second;
		}
		else {
			printf("Start type option not available. Current start type will remain unchanged.");
			start_type = SERVICE_NO_CHANGE;
		};
	}
	else {
		start_type = SERVICE_NO_CHANGE;
	}
	//error type
	DWORD error_control = NULL;
	if (configured_service.error_control_user != "") {
		map<string, DWORD> error_control_type_selection =
		{
			{"normal",SERVICE_ERROR_NORMAL},
			{"severe",SERVICE_ERROR_SEVERE},
			{"critical",SERVICE_ERROR_CRITICAL},
			{"ignore",SERVICE_ERROR_IGNORE}

		};
		map<string, DWORD>::iterator it2;
		it2 = error_control_type_selection.find(configured_service.error_control_user);

		if (it2 != error_control_type_selection.end()) {
			error_control = it2->second;
		}
		else {
			printf("Error control option not available. Error control will remain unchanged");
			error_control = SERVICE_NO_CHANGE;
		};
	}
	else {
		error_control = SERVICE_NO_CHANGE;
	}
	//Configuration Change Start
	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	//opening service
	SC_HANDLE open_service_handler = OpenService(service_control_handler, configured_service.service_name, SERVICE_ALL_ACCESS);
	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}
	//for interact updates
	if (configured_service.service_type_user2 == "interact" && (configured_service.service_type_user == "own" || configured_service.service_type_user == "share")) {
		if (ChangeServiceConfig(open_service_handler, service_type | SERVICE_INTERACTIVE_PROCESS, start_type, error_control, configured_service.binary_path, configured_service.group, configured_service.tag, configured_service.depends, configured_service.ServiceStartName, configured_service.passwd, configured_service.display_name)) {
			printf("Service configuration updated");
			return;
		}
		else {
			printf("Failed to make configuration changes");
			exit(EXIT_FAILURE);
		}
	}
	if (ChangeServiceConfig(open_service_handler, service_type, start_type, error_control, configured_service.binary_path, configured_service.group, configured_service.tag, configured_service.depends, configured_service.ServiceStartName, configured_service.passwd, configured_service.display_name)) {
		printf("Service configuration updated");
	}
	else {
		printf("Failed to make configuration changes");
		exit(EXIT_FAILURE);
	}


}
//Query Service 
void query_my_service(querying_service queried_serv) {

	SERVICE_STATUS_PROCESS service_stats;

	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	SC_HANDLE open_service_handler = OpenService(service_control_handler, queried_serv.service_name, SERVICE_QUERY_STATUS);

	if (open_service_handler == NULL) {
		printf("Failed to open service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}

	//gettting service display name
	TCHAR display_name_holder[256];
	DWORD size_display_name = sizeof(display_name_holder);
	GetServiceDisplayName(open_service_handler, queried_serv.service_name, display_name_holder, &size_display_name);



	if (QueryServiceStatusEx(open_service_handler, SC_STATUS_PROCESS_INFO, (LPBYTE)&service_stats, sizeof(service_stats), (LPDWORD)&queried_serv.bufsize)) {
		cout << "SERVICE_NAME: " << queried_serv.service_name << "\n";
		cout << "DISPLAY_NAME: " << display_name_holder << "\n";
		printf("TYPE: %x\n", service_stats.dwServiceType);
		printf("STATE: %x\n", service_stats.dwCurrentState);
		printf("WIN32_EXIT_CODE: %x\n", service_stats.dwWin32ExitCode);
		printf("SERVICE_EXIT_CODE: %x\n", service_stats.dwServiceSpecificExitCode);
		printf("CHECKPOINT: %x\n", service_stats.dwCheckPoint);
		printf("WAIT_HINT: %x\n", service_stats.dwWaitHint);
		printf("PROCESS_ID: %x\n", service_stats.dwProcessId);
		cout << "\n\n";

	}
	else {

		printf("Failed to query service: %x \n", GetLastError());
		exit(EXIT_FAILURE);
	}



}


void query_my_service_all(querying_service queried_serv) {
	DWORD bytes_n, buffer_req;
	LPDWORD num_of_servics;
	ENUM_SERVICE_STATUS_PROCESS* all_serv_stat;

	SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (service_control_handler == NULL) {
		printf("Failed to open SC Manager: %x \n", GetLastError());
		exit(EXIT_FAILURE);

	}

	LPCSTR conv_group_name = queried_serv.group.c_str();
	if (queried_serv.group == "") {
		conv_group_name = NULL;
	}
	//type 
	DWORD type_val;
	map<string, DWORD> type_selection =
	{
		{"driver",SERVICE_DRIVER},
		{"service",SERVICE_WIN32},
		{"all",SERVICE_DRIVER | SERVICE_WIN32}
	};
	map<string, DWORD>::iterator it3;
	it3 = type_selection.find(queried_serv.type);

	if (it3 != type_selection.end()) {
		type_val = it3->second;
	}
	else {
		printf("Type not available");
	};
	//service type 
	DWORD service_type;
	map<string, DWORD> service_type_selection =
	{
		{"own",SERVICE_WIN32_OWN_PROCESS},
		{"share",SERVICE_WIN32_SHARE_PROCESS},
		{"kernel",SERVICE_KERNEL_DRIVER},
		{"filesys",SERVICE_FILE_SYSTEM_DRIVER},
		{"rec",SERVICE_RECOGNIZER_DRIVER},
		{"interact", SERVICE_INTERACTIVE_PROCESS}

	};
	map<string, DWORD>::iterator it;
	it = service_type_selection.find(queried_serv.service_type);

	if (it != service_type_selection.end()) {
		service_type = it->second;
	}
	else {
		printf("Service type not available");
	};

	//start type
	DWORD state = NULL;
	map<string, DWORD> state_selection =
	{
		{"active",SERVICE_ACTIVE},
		{"inactive",SERVICE_INACTIVE},
		{"all",SERVICE_STATE_ALL}
	};
	map<string, DWORD>::iterator it1;
	it1 = state_selection.find(queried_serv.state);

	if (it1 != state_selection.end()) {
		state = it1->second;
	}
	else {
		printf("State option not available");
	};
	EnumServicesStatusEx(service_control_handler, SC_ENUM_PROCESS_INFO, type_val | service_type, state, NULL, 0, &buffer_req, (LPDWORD)&num_of_servics, 0, conv_group_name);
	bytes_n = buffer_req;
	all_serv_stat = (ENUM_SERVICE_STATUS_PROCESS*)malloc(bytes_n);


	EnumServicesStatusEx(service_control_handler, SC_ENUM_PROCESS_INFO, type_val | service_type, state, (LPBYTE)all_serv_stat, bytes_n, &buffer_req, (LPDWORD)&num_of_servics, NULL, conv_group_name);


	querying_service holding_cell;

	for (int c = 0; c < (int)num_of_servics; c++) {
		holding_cell.service_name = all_serv_stat[c].lpServiceName;
		query_my_service(holding_cell);
	}
	CloseServiceHandle(service_control_handler);

}


//qdescription Service
void qdesc_my_service(qdesc_service desc_service) {

	SERVICE_DESCRIPTION* desc_serv;
	DWORD bytes_req = get_buffer_config_size_desc(desc_service.service_name);
	desc_serv = (SERVICE_DESCRIPTION*)malloc(bytes_req);
	if (desc_serv == NULL) {
		printf("SERVICE NAME: %s\n", desc_service.service_name);
		printf("DESCRIPTION: No Description Available");
	}
	else {
		SC_HANDLE service_control_handler = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

		if (service_control_handler == NULL) {
			printf("Failed to open SC Manager: %x \n", GetLastError());
			exit(EXIT_FAILURE);
		}

		SC_HANDLE open_service_handler = OpenService(service_control_handler, desc_service.service_name, SERVICE_QUERY_CONFIG);

		if (open_service_handler == NULL) {
			printf("Failed to open service: %x \n", GetLastError());
			exit(EXIT_FAILURE);
		}
		if (QueryServiceConfig2(open_service_handler, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)desc_serv, bytes_req, &desc_service.bufsize)) {
			printf("SERVICE NAME: %s\n", desc_service.service_name);
			printf("DESCRIPTION: %s\n", desc_serv->lpDescription);

		}
	}
}


int main(int argc, char** argv) {
	//Create Service
	if (strcmp(argv[1], "create") == 0) {
		if (argc < 5) {
			printf("The correct numbers of parameters was not provided. Must have at least Service Name and the binary path. EX: program name create 'service name' binPath= 'Path'");
			return 1;
		}
		else {
			create_service_i inital_created_service;
			inital_created_service.service_name = argv[2];

			if (_strcmpi(argv[3], "binPath=") != 0) {
				printf("Binary Path (binPath=) must be the argument after the service name");
				return 1;
			}
			inital_created_service.binary_path = argv[4];
			int c = 0;
			//setting any parameters for service
			for (int i = 0; i < argc; i++) {
				if (strcmp(argv[i], "displayname=") == 0) {
					inital_created_service.display_name = argv[i + 1];
				}
				if (strcmp(argv[i], "depend=") == 0) {
					inital_created_service.depends = argv[i + 1];
				}
				if (strcmp(argv[i], "obj=") == 0) {
					inital_created_service.ServiceStartName = argv[i + 1];
				}
				if (strcmp(argv[i], "password=") == 0) {
					inital_created_service.passwd = argv[i + 1];
				}
				if (strcmp(argv[i], "type=") == 0) {
					if (c != 0) {
						inital_created_service.service_type_user2 = "interact";
					}
					else {
						c++;
						inital_created_service.service_type_user = argv[i + 1];
					}
				}
				if (strcmp(argv[i], "start=") == 0) {
					inital_created_service.start_type_user = argv[i + 1];
				}
				if (strcmp(argv[i], "error=") == 0) {
					inital_created_service.error_control_user = argv[i + 1];
				}
				if (strcmp(argv[i], "tag=") == 0) {
					inital_created_service.tag = (LPDWORD)argv[i + 1];
				}
				if (strcmp(argv[i], "group=") == 0) {
					inital_created_service.group = argv[i + 1];
				}

			}
			create_my_service(inital_created_service);
			return 0;
		}
	}
	//Delete Service
	if (strcmp(argv[1], "delete") == 0) {
		delete_service inital_deleted_service;
		inital_deleted_service.service_name = argv[2];
		delete_my_service(inital_deleted_service);
		return 0;
	}
	//qdescription service
	if (strcmp(argv[1], "qdescription") == 0) {
		qdesc_service inital_qdes_service;
		inital_qdes_service.service_name = argv[2];
		qdesc_my_service(inital_qdes_service);
		return 0;
	}
	//Start service
	if (strcmp(argv[1], "start") == 0) {
		starting__stopping_service starting_serv;
		starting_serv.service_name = argv[2];
		start_my_service(starting_serv);
		return 0;
	}
	//Stop service
	if (strcmp(argv[1], "stop") == 0) {
		starting__stopping_service stopping_serv;
		stopping_serv.service_name = argv[2];
		stop_my_service(stopping_serv);
		return 0;
	}
	//Query service
	if (strcmp(argv[1], "query") == 0) {
		querying_service querying_servs;

		//querying a single service
		if (argc == 3) {
			querying_servs.service_name = argv[2];
			query_my_service(querying_servs);
			return 0;
		}

		else {
			for (int i = 0; i < argc; i++) {
				//first type value
				if ((strcmp(argv[i], "type=") == 0) && ((strcmp(argv[i + 1], "driver") == 0) || (strcmp(argv[i + 1], "service") == 0) || (strcmp(argv[i + 1], "all") == 0))) {
					querying_servs.type = argv[i + 1];
				}
				//second type value
				if ((strcmp(argv[i], "type=") == 0) && ((strcmp(argv[i + 1], "own") == 0) || (strcmp(argv[i + 1], "share") == 0) || (strcmp(argv[i + 1], "kernel") == 0) || (strcmp(argv[i + 1], "filesys") == 0) || (strcmp(argv[i + 1], "rec") == 0) || (strcmp(argv[i + 1], "interact") == 0))) {
					querying_servs.service_type = argv[i + 1];
				}
				if (strcmp(argv[i], "state=") == 0) {
					querying_servs.state = argv[i + 1];
				}
				if (strcmp(argv[i], "bufsize=") == 0) {
					querying_servs.bufsize = (DWORD)argv[i + 1];
				}
				if (strcmp(argv[i], "ri=") == 0) {
					querying_servs.ri = (int)argv[i + 1];
				}
				if (strcmp(argv[i], "group=") == 0) {
					querying_servs.group = argv[i + 1];
				}
			}
			query_my_service_all(querying_servs);
			return 0;
		}
		query_my_service_all(querying_servs);
		return 0;
	}
	//Failure service
	if ((strcmp(argv[1], "failure") == 0) && argc < 4) {
		printf("Not enough arguments provided");
		return 1;
	}
	if ((strcmp(argv[1], "failure") == 0) && argc > 4) { 
		config_failure_service cfig_fail;
		cfig_fail.service_name = argv[2];

		//Failure parameters
		for (int i = 0; i < argc; i++) {
			if (strcmp(argv[i], "reset=") == 0) {
				cfig_fail.reset_period = (DWORD)argv[i + 1];
			}
			if (strcmp(argv[i], "reboot=") == 0) {
				cfig_fail.reboot_msg = argv[i + 1];
			}
			if (strcmp(argv[i], "command=") == 0) {
				cfig_fail.command = argv[i + 1];
			}
			if (strcmp(argv[i], "actions=") == 0) {
				cfig_fail.actions = argv[i + 1];
			}
		}
		failure_my_service(cfig_fail);
		return 0;
	}
	//Config service
	if ((strcmp(argv[1], "config") == 0) && argc < 3) {
		printf("Not enough arguments provided");
		return 1;
	}
	if ((strcmp(argv[1], "config") == 0) && argc > 3) {
		config_service_i cfig_serv;
		cfig_serv.service_name = argv[2];

		int c = 0;
		//setting parameters to configured
		for (int i = 0; i < argc; i++) {
			if (strcmp(argv[i], "displayname=") == 0) {
				cfig_serv.display_name = argv[i + 1];
			}
			if (strcmp(argv[i], "depends=") == 0) {
				cfig_serv.depends = argv[i + 1];
			}
			if (strcmp(argv[i], "obj=") == 0) {
				cfig_serv.ServiceStartName = argv[i + 1];
			}
			if (strcmp(argv[i], "password=") == 0) {
				cfig_serv.passwd = argv[i + 1];
			}
			if (strcmp(argv[i], "type=") == 0) {
				if (c != 0) {
					cfig_serv.service_type_user2 = "interact";
				}
				else {
					c++;
					cfig_serv.service_type_user = argv[i + 1];
				}
			}
			if (strcmp(argv[i], "start=") == 0) {
				cfig_serv.start_type_user = argv[i + 1];
			}
			if (strcmp(argv[i], "error=") == 0) {
				cfig_serv.error_control_user = argv[i + 1];
			}
			if (strcmp(argv[i], "tag=") == 0) {
				cfig_serv.tag = (LPDWORD)argv[i + 1];
			}
			if (strcmp(argv[i], "binPath=") == 0) {
				cfig_serv.binary_path = argv[i + 1];
			}
			if (strcmp(argv[i], "group=") == 0) {
				cfig_serv.group = argv[i + 1];
			}
		}
		configure_my_service(cfig_serv);
		return 0;
	}
	else {
		printf("Argument entered is not available for this program");
		return 1;
	}
}