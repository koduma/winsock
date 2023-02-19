#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

using namespace std;

int main() {

	char server_ip_addr[32];
	int port_number;
	
	// Windows Sockets仕様に関する情報を格納する構造体
	WSADATA wsa_data;
	// WinSockの初期化処理(Version 2.0)
	if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
		cerr << "Winsockの初期化失敗(WSAStartup)" << endl;
	}

	// ユーザ入力
	cout << "接続先IPアドレスを入力してください(xxx.xxx.xxx.xxx)" << endl;
	cin >> server_ip_addr;
	cout << "ポート番号を入力してください" << endl;
	cin >> port_number;

	// sockaddr_in構造体の作成とポート番号、IPタイプの入力
	struct sockaddr_in dst_addr;
	memset(&dst_addr, 0, sizeof(dst_addr));
	dst_addr.sin_port = htons(port_number);		// ポート番号
	dst_addr.sin_family = AF_INET;				// AF_INETはipv4を示す

	// 引数は (1) Type(ipv4 or v6) (2) IPアドレスのテキスト形式 (3) IPアドレスのバイナリ形式【(2)→(3)に変換】
	inet_pton(dst_addr.sin_family, server_ip_addr, &dst_addr.sin_addr.s_addr);

	// AF_INETはipv4のIPプロトコル & SOCK_STREAMはTCPプロトコル
	int dst_socket = socket(AF_INET, SOCK_STREAM, 0);

	// 接続処理
	if (connect(dst_socket, (struct sockaddr *) &dst_addr, sizeof(dst_addr))) {
		cerr << "接続失敗(サーバIPアドレス" << server_ip_addr << "/接続先ポート番号" << port_number << endl;
		exit(0);
	}

	cout << "接続完了(サーバIPアドレス" << server_ip_addr << "/接続先ポート番号" << port_number << endl << endl;;
	
	
	char send_buf1[256], send_buf2[256];
	char recv_buf[256];

	while (1) {
		
		cout << "数字を2個入力してください" << endl;
		cin >> send_buf1 >> send_buf2;
		
		// Packetの送信(SOCKET, Buffer, Datasize, 送信方法)
		send(dst_socket, send_buf1, 256, 0);
		send(dst_socket, send_buf2, 256, 0);

		// Packetの受信
		recv(dst_socket, recv_buf, 256, 0);

		// 受信結果の表示
		cout << "合計は" << atoi(recv_buf) << endl << endl;
	}

	// 解放処理
	closesocket(dst_socket);

	// WinSockの終了処理
	WSACleanup();
	return 0;
}
