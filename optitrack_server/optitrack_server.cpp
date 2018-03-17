#pragma comment(lib,"ws2_32.lib")

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <Windows.h>
#include <dos.h>
#include "vrpn.h"

using namespace std;

int status;
int i = 1;
float Datos[] = {0,0,0,0,0,0};
//double doubleDatos[] = {1.2,3.4,5.6};
//char num[sizeof(double)];
char paquete[sizeof(Datos)];
int cont = 0;
int portNum = 444;

int main()
{
	/*cout << sizeof(Datos) << " " << sizeof(paquete) << endl;
	system("pause");
	return 0;*/
	//Conexión con VRPN
	#pragma region ConexionVRPN
		sprintf(conexionNom, "localhost:%d", DEFAULT_VRPN_PORT);
		conexion = vrpn_get_connection_by_name(conexionNom);

		vrpn_Tracker_Remote *tracker1= new vrpn_Tracker_Remote("tracker1", conexion);
		//vrpn_Tracker_Remote *tracker2= new vrpn_Tracker_Remote("tracker2", conexion);

		tracker1->register_change_handler(NULL, handle_pos1);
		//tracker2->register_change_handler(NULL, handle_pos2);

		puts("\nComuncacion VRPN iniciada.\n");
		Sleep(1000);
	#pragma endregion
	
	tracker1->mainloop();
	conexion->mainloop();

	//Creacion del SOCKET
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2,1);
	if(WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL,"Error iniciando winsock...", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);
	addr.sin_addr.s_addr = INADDR_ANY;
	//addr.sin_addr.s_addr = inet_addr("192.168.2.102");
	addr.sin_port = htons(portNum);
	addr.sin_family = AF_INET;

	cout << "Creando Socket..." << endl;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	cout << "Escuchando..." << endl;
	listen(sListen, SOMAXCONN);

	SOCKET newConnection;
	newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);

	if(newConnection == 0){
		cout << "Falla al crear conexion con el cliente..." << endl;
		exit(1);
	} else {
		cout << "Conexion con el cliente exitosa: " << addr.sin_addr.s_addr << endl;
	}

	// && status  != -1
	while(!GetAsyncKeyState(27) && status  != -1){
		tracker1->mainloop();
		Datos[0] = (float) xl;
		Datos[1] = (float) yl;
		Datos[2] = (float) zl;
		Datos[3] = (float) yawl;
		Datos[4] = (float) roll;
		Datos[5] = (float) pitch;

		memcpy(paquete,Datos,sizeof(Datos));
		status = send(newConnection, paquete, sizeof(paquete), 0);
		cout << cont;
		for(int j = 0; j < (sizeof(Datos)/sizeof(int)); j++){
			 cout << "  " << Datos[j];
		}
		cout << endl;
		cont++;
		Sleep(5);
	}
	system("pause");
	return (0);
}