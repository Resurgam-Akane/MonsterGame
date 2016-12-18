#include "Protocol.h"
#include "Player.h"
#include "Driver.h"
//todo: driver 全局变量
namespace Protocol
{
	std::string CommandDealForClient(std::string s)
	{
		if (s.substr(0, 2) == "98") { 
			std::string a;
			return std::string{ "注册成功" }; 
		}
		else if (s.substr(0, 2) == "97") { 
			return std::string{ "注册失败,用户名已存在" }; 
		}
		else if (s.substr(0, 2) == "95") {
			std::string a;
			a = s.substr(3, s.length() - 3);
			Driver::user = Player::Player(a);
			Driver::IsLogin = true;
			return std::string{ "登录成功" };
		}
		else if (s.substr(0, 2) == "94") {
			if (s[3] == '1') return std::string{ "密码错误" };
			else if (s[3] == '2') return std::string{ "不存在该用户" };
		}
		else if (s.substr(0, 2) == "00") {
			std::string a;
			a = s.substr(3, s.length() - 3);
			int index = a.find_first_of('|');
			Driver::RoomNum = a.substr(0, index);
			Driver::ServerWarMonster = Driver::CreateMonsterFromInfo(a.substr(index + 1, a.length() - index - 1));
			Driver::IsRequest = true;
			return std::string{ "请求对战成功" };
		}
		else if (s.substr(0, 2) == "01") {
			//todo:服务器先手
			Driver::IsReady = true;
			std::cout << "服务器先手" << std::endl;
			Driver::ClientChangeStateForServerFirst(s.substr(3, s.length() - 3));
			return "服务器攻击完毕，客户端现在选择攻击";
		}
		else if (s.substr(0, 2) == "02") {
			Driver::IsReady = true;
			return std::string{ "客户端先攻击" };
		}
		else if (s.substr(0, 2) == "04") {
			std::string result = Driver::ClientChangeState(s.substr(3, s.length() - 3));
			if (result == "胜利") {
				Driver::IsRequest = false;
				Driver::IsReady = false;
				return result;
			}
			else if (result == "失败") {
				Driver::IsRequest = false;
				Driver::IsReady = false;
				return result;
			}
			return "服务器攻击完毕，客户端现在选择攻击";
		}
		else if (s.substr(0, 2) == "90") {
			return "同步成功";
		}
		else if (s.substr(0, 2) == "88") {
			std::string a;    //s = "95|admin|4|29|1|M5S10B5D5A5R0E0T0|"
			a = s.substr(3, s.length() - 3);
			Driver::user = Player::Player(a);
			return "同步成功";
		}
		else if (s.substr(0,2) == "85"){
			Driver::OnlineUserInfo = s.substr(3, s.length() - 3);
			return "返回在线用户成功";
		}
	}
}
