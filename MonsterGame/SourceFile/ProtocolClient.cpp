#include "Protocol.h"
#include "Player.h"
#include "Driver.h"
//todo: driver ȫ�ֱ���
namespace Protocol
{
	std::string CommandDealForClient(std::string s)
	{
		if (s.substr(0, 2) == "98") { 
			std::string a;
			return std::string{ "ע��ɹ�" }; 
		}
		else if (s.substr(0, 2) == "97") { 
			return std::string{ "ע��ʧ��,�û����Ѵ���" }; 
		}
		else if (s.substr(0, 2) == "95") {
			std::string a;
			a = s.substr(3, s.length() - 3);
			Driver::user = Player::Player(a);
			Driver::IsLogin = true;
			return std::string{ "��¼�ɹ�" };
		}
		else if (s.substr(0, 2) == "94") {
			if (s[3] == '1') return std::string{ "�������" };
			else if (s[3] == '2') return std::string{ "�����ڸ��û�" };
		}
		else if (s.substr(0, 2) == "00") {
			std::string a;
			a = s.substr(3, s.length() - 3);
			int index = a.find_first_of('|');
			Driver::RoomNum = a.substr(0, index);
			Driver::ServerWarMonster = Driver::CreateMonsterFromInfo(a.substr(index + 1, a.length() - index - 1));
			Driver::IsRequest = true;
			return std::string{ "�����ս�ɹ�" };
		}
		else if (s.substr(0, 2) == "01") {
			//todo:����������
			Driver::IsReady = true;
			std::cout << "����������" << std::endl;
			Driver::ClientChangeStateForServerFirst(s.substr(3, s.length() - 3));
			return "������������ϣ��ͻ�������ѡ�񹥻�";
		}
		else if (s.substr(0, 2) == "02") {
			Driver::IsReady = true;
			return std::string{ "�ͻ����ȹ���" };
		}
		else if (s.substr(0, 2) == "04") {
			std::string result = Driver::ClientChangeState(s.substr(3, s.length() - 3));
			if (result == "ʤ��") {
				Driver::IsRequest = false;
				Driver::IsReady = false;
				return result;
			}
			else if (result == "ʧ��") {
				Driver::IsRequest = false;
				Driver::IsReady = false;
				return result;
			}
			return "������������ϣ��ͻ�������ѡ�񹥻�";
		}
		else if (s.substr(0, 2) == "90") {
			return "ͬ���ɹ�";
		}
		else if (s.substr(0, 2) == "88") {
			std::string a;    //s = "95|admin|4|29|1|M5S10B5D5A5R0E0T0|"
			a = s.substr(3, s.length() - 3);
			Driver::user = Player::Player(a);
			return "ͬ���ɹ�";
		}
		else if (s.substr(0,2) == "85"){
			Driver::OnlineUserInfo = s.substr(3, s.length() - 3);
			return "���������û��ɹ�";
		}
	}
}
