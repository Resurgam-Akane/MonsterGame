#pragma once
#include "EggAche\EggAche.h"
#include "MsgSocket.h"
#include "Protocol.h"
#include "Driver.h"
#include <string>
#include <functional>
#include <mutex>
#include <vector>

namespace GUI
{

	struct Rect {
		size_t x, y, w, h;
	};

	class Button
	{
	public:
		Button(EggAche::Canvas &canvas,
			const std::string &text,
			size_t x, size_t y,
			size_t w = 20, size_t h = 18)
			: _rect{ x, y, w, h }
		{
			canvas.DrawTxt(_rect.x + _rect.w / 2,
				_rect.y + (_rect.h - 18) / 2,
				text.c_str());

			_rect.w += canvas.GetTxtWidth(text.c_str());
			canvas.DrawRdRt(_rect.x, _rect.y,
				_rect.x + _rect.w,
				_rect.y + _rect.h,
				_rect.w / 4, _rect.h / 4);
		}

		bool TestClick(size_t x, size_t y)
		{
			return x >= _rect.x && y >= _rect.y &&
				x <= _rect.x + _rect.w &&
				y <= _rect.y + _rect.h;
		}

	private:
		Rect _rect;
	};

	class Input
	{
	public:
		Input(EggAche::Canvas &canvas,
			const std::string &text,
			const std::string &hintText,
			size_t x, size_t y,
			size_t w, size_t h = 18)
			: _canvas(canvas), _rect{ x, y, w + 4, h + 2 },
			_text(text), _hintText(hintText), _isActivated(false)
		{
			ReDraw();
		}

		bool TestClick(size_t x, size_t y)
		{
			return x >= _rect.x && y >= _rect.y &&
				x <= _rect.x + _rect.w &&
				y <= _rect.y + _rect.h;
		}

		bool isActivated() const
		{
			return _isActivated;
		}

		void Activate(bool isActivated = true)
		{
			_isActivated = isActivated;
			ReDraw();
		}

		const std::string &GetText() const
		{
			return _text;
		}

		void InputKey(char ch)
		{
			if (!_isActivated)
				return;

			if (ch == '\x08' && !_text.empty())
				_text.pop_back();
			else if ((isalnum(ch) || ch == '_') && _text.size() < 16)
				_text.push_back(ch);

			ReDraw();
		}

	private:
		EggAche::Canvas &_canvas;
		Rect _rect;
		std::string _hintText;
		bool _isActivated;
		std::string _text;

		void ReDraw()
		{
			_canvas.SetBrush(false, 255, 255, 255);
			_canvas.DrawRect(_rect.x, _rect.y,
				_rect.x + _rect.w,
				_rect.y + _rect.h);
			_canvas.SetBrush(true, 0, 0, 0);

			const auto &strToDraw =
				_text.empty() && !_isActivated ? _hintText : _text;
			_canvas.DrawTxt(_rect.x + 2, _rect.y + 1,
				strToDraw.c_str());

			if (_isActivated)
			{
				auto posX = _rect.x + 2 +
					_canvas.GetTxtWidth(strToDraw.c_str());
				_canvas.DrawLine(posX, _rect.y + 2,
					posX, _rect.y - 2 + _rect.h);
			}
		}
	};


	class LoginWindow
	{
	public:
		using callback = std::function<bool(const std::string&, const std::string&)>;
		LoginWindow(size_t width = 960, size_t height = 540);
		void LoginWindowInit();

		bool IsClosed() const
		{
			return _wnd.IsClosed();
		}

		void Prompt(const std::string &prompt)
		{
			_prompt->Clear();
			_prompt->DrawTxt(0, 0, prompt.c_str());
		}

		void ClickRegister(callback fn)
		{
			_onRegister = std::move(fn);
		}

		void ClickLogin(callback fn)
		{
			_onLogin = std::move(fn);
		}
	private:
		EggAche::Window _wnd;
		std::unique_ptr<EggAche::Canvas> _bgCanvas, _prompt;
		std::unique_ptr<Button> _loginBtn, _registerBtn;
		std::unique_ptr<Input> _usernameInput, _pwdInput;
		size_t _width, _height;
		std::string _username_before, _pwd_before;
		callback _onLogin, _onRegister;
	};

	class UserInfoWindow
	{
	public:
		using callback = std::function<bool(void)>;
		UserInfoWindow(size_t width = 1344, size_t height = 756);
		void UserInfoWindowInit();
		void ClickPveBtn(callback fn)
		{
			_onPveBtn = std::move(fn);
		}

		void ClickOnlineUserBtn(callback fn)
		{
			_onOnlineUserBtn = std::move(fn);
		}

		bool IsClosed() const
		{
			return _wnd.IsClosed();
		}

	private:
		EggAche::Window _wnd;
		std::unique_ptr<EggAche::Canvas> _bgCanvas;
		std::unique_ptr<Button> _pveBtn, _onlineUserBtn;
		size_t _width, _height;
		callback _onPveBtn, _onOnlineUserBtn;
	};


	class SelectMonsterWindow
	{
	public:
		using callback = std::function<bool(void)>;
		using selectmonster = std::function<bool(int&)>;
		SelectMonsterWindow(size_t width = 1344, size_t height = 756);
		void SelectMonsterWindowInit();

		void ClickReturnBtn(callback fn)
		{
			_onReturnBtn = std::move(fn);
		}

		void ClickMonserVecBtn(selectmonster fn)
		{
			_onMonsterVecBtn = std::move(fn);
		}

		bool IsClosed() const
		{
			return _wnd.IsClosed();
		}

		~SelectMonsterWindow()
		{
			
		}
	private:
		EggAche::Window _wnd;
		std::unique_ptr<EggAche::Canvas> _bgCanvas;
		std::vector<std::unique_ptr<Button>> _monsterVecBtn;
		std::unique_ptr<Button> _returnBtn;
		size_t _width, _height;
		callback _onReturnBtn;
		selectmonster _onMonsterVecBtn;
	};

	class WarWindow
	{
	public:
		using callback = std::function<bool()>;
		using useSkill = std::function<bool(int&)>;
		WarWindow(size_t width = 1200, size_t height = 741);
		void WarWindowInit();

		bool IsClosed() const
		{
			return _wnd.IsClosed();
		}

		void ClickReadyBtn(callback fn)
		{
			_onReadyBtn = std::move(fn);
		}

		void ClickAttackBtn(callback fn)
		{
			_onAttackBtn = std::move(fn);
		}

		void ClickSkillBtn(useSkill fn)
		{
			_onSkillBtn = std::move(fn);
		}

		void PromptServerBlood(const std::string& prompt)
		{
			if (_prompt_serverblood != nullptr) {
				_prompt_serverblood->Clear();
				_prompt_serverblood->DrawTxt(0, 0, std::string{ "ServerBlood:"  + prompt }.c_str());
			}
		}

		void PromptServerEnergy(const std::string& prompt)
		{
			if (_prompt_serverenergy != nullptr) {
				_prompt_serverenergy->Clear();
				_prompt_serverenergy->DrawTxt(0, 0, std::string{ "ServerEnergy:" + prompt }.c_str());
			}
		}

		void PromptClientBlood(const std::string& prompt)
		{
			if (_prompt_clientblood != nullptr) {
				_prompt_clientblood->Clear();
				_prompt_clientblood->DrawTxt(0, 0, std::string{ "ClientBlood:" + prompt }.c_str());
			}
		}

		void PromptClientEnergy(const std::string& prompt)
		{
			if (_prompt_clientenergy != nullptr) {
				_prompt_clientenergy->Clear();
				_prompt_clientenergy->DrawTxt(0, 0, std::string{"ClientEnergy:" + prompt}.c_str());
			}
		}

		void RightTOLeft();
		void LeftTORight();

		bool right2leftFirst;
		bool animation;

	private:
		EggAche::Window _wnd;
		std::unique_ptr<EggAche::Canvas> _bgCanvas;
		std::unique_ptr<EggAche::Canvas> _prompt_serverblood, _prompt_serverenergy, _prompt_clientblood, _prompt_clientenergy;
		std::unique_ptr<EggAche::Canvas> _rightToLeftAnimation, _leftToRightAnimation;
		std::unique_ptr<Button> _readyBtn, _attackBtn;
		std::vector<std::unique_ptr<Button>> _skillBtn;
		size_t _width, _height;
		int _ServerMosnterID, _ClientMonsterID;
		callback _onReadyBtn, _onAttackBtn;
		useSkill _onSkillBtn;

		void _DrawMonster(std::unique_ptr<EggAche::Canvas>& bg, int id, size_t x_pot, size_t y_pos);
	};


	class WinSettleAccountWindow
	{
	public:
		using callback = std::function<bool()>;
		using selectmonster = std::function<bool(int&)>;
		WinSettleAccountWindow(size_t width = 1344, size_t height = 672);
		void WinSettleAccountWindowInit();

		bool IsClosed() const
		{
			return _wnd.IsClosed();
		}

		//todo: 本地与服务器同步
		void ClickReturnBtn(callback fn)
		{
			_onReturnBtn = std::move(fn);
		}

		int GetMonsterNum()
		{
			return _monsternum;
		}

		std::string GetRoomNum()
		{
			return _roomnum;
		}

		std::string GetUsername()
		{
			return _Username;
		}

	private:
		EggAche::Window _wnd;
		std::unique_ptr<EggAche::Canvas> _bgCanvas;
		std::unique_ptr<Button> _returnBtn;
		std::vector<std::unique_ptr<Button>> _monsterVecBtn;
		size_t _width, _height;
		std::string _roomnum, _Username;
		int _monsternum;
		callback _onReturnBtn;
		selectmonster _onMonsterVecBtn;

	};

	
	class FailSettleAccountWindow
	{
	public:
		using callback = std::function<bool(int&)>;
		FailSettleAccountWindow(size_t width = 1344, size_t height = 672);
		void FailSettleAccountWindowInit();

		bool IsClosed() const
		{
			return _wnd.IsClosed();
		}

		void ClickMonsterVecBtn(callback fn)
		{
			_onMonsterVecBtn = std::move(fn);
		}

		std::string GetRoomNum()
		{
			return _roomnum;
		}

		std::string GetUsername()
		{
			return _username;
		}
	private:
		EggAche::Window _wnd;
		std::unique_ptr<EggAche::Canvas> _bgCanvas;
		std::vector<std::unique_ptr<Button>> _monsterVecBtn;
		std::string _roomnum, _username;
		size_t _width, _height;
		callback _onMonsterVecBtn;
	};

	class OnlineUserWindow
	{
	public:
		using callback = std::function<bool(void)>;
		OnlineUserWindow(size_t width = 1200, size_t height = 732);
		void OnlineUserWindowInit();

		bool IsClosed() const
		{
			return _wnd.IsClosed();
		}

		void ClickReturnBtn(callback fn)
		{
			_onReturnBtn = std::move(fn);
		}
	private:
		EggAche::Window _wnd;
		std::unique_ptr<EggAche::Canvas> _bgCanvas;
		std::unique_ptr<Button> _returnBtn;
		size_t _width, _height;
		callback _onReturnBtn;
	};


	class ClientGUI
	{
	public:
		ClientGUI()
		{
			;
		}
		bool Login()
		{
			bool hasLogin = false;
			LoginWindow wnd(960,540);

			std::mutex mtx;

			wnd.ClickLogin([&wnd, this, &mtx, &hasLogin](const std::string &username,const std::string& pwd)
			{
				std::cout << "登录加锁" << std::endl;
				std::lock_guard<std::mutex> lck(mtx);
				wnd.Prompt("Loging in...");

				this->_content = "96|" + username + "|" + pwd;
				this->_content = Client.ClientWork(this->_content);
				std::string backinfo = Protocol::CommandDealForClient(this->_content);
				if (backinfo == "登录成功") {
					wnd.Prompt("Login successfully");
					hasLogin = true;
					return false;
				}
				else if (backinfo == "密码错误") {
					wnd.Prompt("Password is wrong");
				}
				else if (backinfo == "不存在该用户")
					wnd.Prompt("Username is wrong");

				std::cout << "登录解锁" << std::endl;
				return true;
			});

			wnd.ClickRegister([&wnd, this, &mtx](const std::string &username, const std::string& pwd) 
			{
				std::cout << "注册加锁" << std::endl;
				std::lock_guard<std::mutex> lck(mtx);
				wnd.Prompt("Registering...");
				this->_content = "99|" + username + "|" + pwd;
				this->_content = Client.ClientWork(this->_content);
				std::string backinfo = Protocol::CommandDealForClient(this->_content);
				if (backinfo == "注册成功") {
					wnd.Prompt("Register successfully");
				}
				else if (backinfo == "注册失败,用户名已存在")
				wnd.Prompt("Username has already existed.");
				std::cout << "注册解锁" << std::endl;
				return true;
			});

			while (!wnd.IsClosed() && !hasLogin) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			return Driver::IsLogin = hasLogin;
		}

		int UserInfo()
		{
			int direction = -1;
			bool hasrequest = false;
			UserInfoWindow wnd(1344, 756);

			std::mutex mtx;

			wnd.ClickPveBtn([&wnd, this, &mtx, &hasrequest, &direction]()
			{
				std::lock_guard<std::mutex> lck(mtx);
				hasrequest = true;
				direction = 0;
				return false;
			});

			wnd.ClickOnlineUserBtn([&wnd, this, &mtx, &hasrequest, &direction]()
			{
				std::lock_guard<std::mutex> lck(mtx);
				hasrequest = false;
				direction = 1;
				this->_content = "85";
				this->_content = Client.ClientWork(this->_content);
				std::string backinfo = Protocol::CommandDealForClient(this->_content);
				if (backinfo == "返回在线用户成功")
					return false;
				return true;
			});

			while (!wnd.IsClosed() && (direction == -1)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			Driver::HasRequest = hasrequest;
			return direction;
		}

		int OnlineUser()
		{
			int direction = -1;
			OnlineUserWindow wnd(1200,732);

			std::mutex mtx;
			wnd.ClickReturnBtn([&wnd, this, &mtx, &direction]()
			{
				std::lock_guard<std::mutex> lck(mtx);
				direction = 0; //return to userinfo
				return false;
			});

			while (!wnd.IsClosed() && (direction == -1)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			return direction;
		}

		int SelectMonster()
		{
			int direction = -1;
			SelectMonsterWindow wnd(1344, 756);

			std::mutex mtx;
			wnd.ClickReturnBtn([&wnd, this, &mtx, &direction]()
			{
				std::lock_guard<std::mutex> lck(mtx);
				direction = 0; //return to userinfo
				return false;
			});

			wnd.ClickMonserVecBtn([&wnd, this, &mtx, &direction](int &i)
			{
				std::lock_guard<std::mutex> lck(mtx);
				Driver::ClientWarMonster = Driver::user.Get_A_Monster(i + 1);
				Driver::ClientWarMonster->SetEnergy(0);
				std::string MonsterInfo = Driver::user.GetMonsterInfo(i + 1);
				this->_content = "00|" + MonsterInfo;
				this->_content = Client.ClientWork(this->_content);
				std::string backinfo = Protocol::CommandDealForClient(this->_content);
				if (backinfo == "请求对战成功") {
					direction = 1; // request war success.
					return false;
				}
				else {
					direction = 2;							  // request war failed.
					return false;
				}
			});

			while (!wnd.IsClosed() && (direction == -1)) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			return direction;
		}

		bool War()
		{
			bool isEnd = false;
			bool isReady = false;
			bool isWin = false;
			WarWindow wnd(1200, 741);
			wnd.animation = false;

			std::mutex mtx;

			wnd.ClickReadyBtn([&wnd, this, &mtx, &isEnd, &isReady]()
			{
				std::cout << "ready加锁" << std::endl;
				std::lock_guard<std::mutex> lck(mtx);
				if (isReady == false) {
					wnd.PromptClientBlood(std::to_string(Driver::ClientWarMonster->GetCurBlood()));
					wnd.PromptClientEnergy(std::to_string(Driver::ClientWarMonster->GetEnergy()));
					this->_content = Client.ClientWork("01|" + Driver::RoomNum + "|");
					std::string backinfo = Protocol::CommandDealForClient(this->_content);
					if (backinfo == "服务器攻击完毕，客户端现在选择攻击") {
						isReady = true;
						wnd.animation = true;
						//wnd.LeftTORight();
						wnd.right2leftFirst = false;
						wnd.PromptClientBlood(std::to_string(Driver::ClientWarMonster->GetCurBlood()));
						wnd.PromptClientEnergy(std::to_string(Driver::ClientWarMonster->GetEnergy()));
						wnd.PromptServerBlood(std::to_string(Driver::ServerWarMonster->GetCurBlood()));
						wnd.PromptServerEnergy(std::to_string(Driver::ServerWarMonster->GetEnergy()));
					}
					else if (backinfo == "客户端先攻击") {
						wnd.PromptClientBlood(std::to_string(Driver::ClientWarMonster->GetCurBlood()));
						wnd.PromptClientEnergy(std::to_string(Driver::ClientWarMonster->GetEnergy()));
						wnd.PromptServerBlood(std::to_string(Driver::ServerWarMonster->GetCurBlood()));
						wnd.PromptServerEnergy(std::to_string(Driver::ServerWarMonster->GetEnergy()));
						isReady = true;
						return false;
					}
				}
				std::cout << "ready解锁" << std::endl;
				return true;
			});

			wnd.ClickAttackBtn([&wnd, this, &mtx, &isEnd, &isReady, &isWin]()
			{
				std::cout << "普通攻击加锁" << std::endl;
				std::lock_guard<std::mutex> lck(mtx);
				
				if (isReady == true) {
					this->_content = "02|" + Driver::RoomNum + "|At" + "|" + std::to_string(Driver::ClientWarMonster->GetEnergy());
					this->_content = Client.ClientWork(this->_content);
					std::string backinfo = Protocol::CommandDealForClient(this->_content);
					wnd.animation = true;
					wnd.right2leftFirst = true;
					//wnd.RightTOLeft();
					wnd.PromptServerBlood(std::to_string(Driver::ServerWarMonster->GetCurBlood()));
					wnd.PromptServerEnergy(std::to_string(Driver::ServerWarMonster->GetEnergy()));
					//wnd.LeftTORight();
					wnd.PromptClientBlood(std::to_string(Driver::ClientWarMonster->GetCurBlood()));
					wnd.PromptClientEnergy(std::to_string(Driver::ClientWarMonster->GetEnergy()));
					if (backinfo == "胜利") {
						isEnd = true;
						isWin = true;
						return false;
					}
					else if (backinfo == "失败") {
						isEnd = true;
						isWin = false;
						return false;
					}
					else {
						isEnd = false;
						return true;
					}
				}
//				else EggAche::MsgBox("Please be ready.", "MonsterGame");
				std::cout << "普通攻击解锁" << std::endl;
				return true;
			});

			wnd.ClickSkillBtn([&wnd, this, &mtx, &isEnd, &isReady, &isWin](int& i)
			{
				std::cout << "使用技能加锁" << std::endl;
				std::lock_guard<std::mutex> lck(mtx);
				if (isReady == true) {
					this->_content = "02|" + Driver::RoomNum + "|" + Driver::ClientWarMonster->Attack(i+1) + "|" + std::to_string(Driver::ClientWarMonster->GetEnergy());
					this->_content = Client.ClientWork(this->_content);
					std::string backinfo = Protocol::CommandDealForClient(this->_content);
					wnd.animation = true;
					wnd.right2leftFirst = true;
					//wnd.RightTOLeft();
					wnd.PromptServerBlood(std::to_string(Driver::ServerWarMonster->GetCurBlood()));
					wnd.PromptServerEnergy(std::to_string(Driver::ServerWarMonster->GetEnergy()));
					//wnd.LeftTORight();
					wnd.PromptClientBlood(std::to_string(Driver::ClientWarMonster->GetCurBlood()));
					wnd.PromptClientEnergy(std::to_string(Driver::ClientWarMonster->GetEnergy()));
					if (backinfo == "胜利") {
						isEnd = true;
						isWin = true;
						return false;
					}
					else if (backinfo == "失败") {
						isEnd = true;
						isWin = false;
						return false;
					}
					else {
						isEnd = false;
						return true;
					}
				}
//				else EggAche::MsgBox("Please be ready.", "MonsterGame");
				std::cout << "使用技能解锁" << std::endl;
				return true;
			});


			while (!wnd.IsClosed() && !isEnd) {
				if (wnd.animation) {
					if (!wnd.right2leftFirst)
						wnd.LeftTORight();
					if (wnd.right2leftFirst) {
						wnd.RightTOLeft();
						wnd.LeftTORight();
					}
					wnd.animation = false;
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			{
				std::lock_guard<std::mutex> lg(mtx);
			}

			return Driver::IsWin = isWin;
		}

		bool WinSettleAccount()
		{
			bool isReturn = false;
			WinSettleAccountWindow wnd(1344,672);
			std::mutex mtx;

			wnd.ClickReturnBtn([&wnd, this, &mtx, &isReturn]()
			{
				std::cout << "返回加锁" << std::endl;
				std::lock_guard<std::mutex> lck(mtx);	
				Driver::ClientWarMonster->Levelup(40);
				Driver::user.AddWinNum();
				if (wnd.GetMonsterNum() < 9) {
					Driver::ServerWarMonster->SetBlood(Driver::ServerWarMonster->GetCalBlood());
					Driver::user.GetMonster(Driver::ServerWarMonster);
					this->_content = "90|" + wnd.GetRoomNum() + "|" + wnd.GetUsername() + "|" + std::to_string(Driver::user.GetWinNum()) + "|" + std::to_string(Driver::user.GetFailureNum()) + "|"  + Driver::user.MonsterList(); // win, get a monster
				}
				else this->_content = "90|" + wnd.GetRoomNum() + "|" + wnd.GetUsername() + "|" + std::to_string(Driver::user.GetWinNum()) + "|" + std::to_string(Driver::user.GetFailureNum()) + "|" + Driver::user.MonsterList(); // win, not get a monster
				this->_content = Client.ClientWork(this->_content);
				std::string backinfo = Protocol::CommandDealForClient(this->_content);
				if (backinfo == "同步成功") {
					isReturn = true;
					return false;
				}
				std::cout << "返回解锁" << std::endl;
				return true;
			});

			while (!wnd.IsClosed() && !isReturn) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			return isReturn;
		}

		bool FailSettleAccount()
		{
			int los = 0;
			bool isReturn = false;
			FailSettleAccountWindow wnd(1344, 672);
			std::mutex mtx;

			wnd.ClickMonsterVecBtn([&wnd, this, &mtx, &isReturn, &los](int&i)
			{
				std::lock_guard<std::mutex> lck(mtx);
				Driver::ClientWarMonster->Levelup(20);
				los = i;
				isReturn = true;
				Driver::user.AddFailNum();

				if (Driver::user.GetMonsterNum() == 1) {
					Driver::user.LostMonster(i);
					this->_content = "88|" + wnd.GetRoomNum() + "|" + wnd.GetUsername() + "|" + std::to_string(Driver::user.GetWinNum()) + "|" + std::to_string(Driver::user.GetFailureNum()); // fail, 3 new
				}
				else {
					Driver::user.LostMonster(i);
					this->_content = "89|" + wnd.GetRoomNum() + "|" + wnd.GetUsername() + "|" + std::to_string(Driver::user.GetWinNum()) + "|" + std::to_string(Driver::user.GetFailureNum()) + "|" + Driver::user.MonsterList(); 
				}
				this->_content = Client.ClientWork(this->_content);
				std::string backinfo = Protocol::CommandDealForClient(this->_content);
				if (backinfo == "同步成功") {
					isReturn = true;
					return false;
				}
				return true;
			});

			while (!wnd.IsClosed() && !isReturn) {
				std::this_thread::sleep_for(std::chrono::milliseconds(200));
			}

			{
				std::lock_guard<std::mutex> lg(mtx);
			}

			return isReturn;
		}

		~ClientGUI()
		{
			Socket.~MsgSocketInit();
		}
	private:
		std::string _content;
		MsgSocket::MsgSocketInit Socket;
		MsgSocket::Client Client;

	};
	
}