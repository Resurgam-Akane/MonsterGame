#include "GUIGame.h"
#include <sstream>

namespace GUI
{
	std::string DoubleToString(double d)
	{
		std::stringstream ss;
		std::string str;

		ss << d;
		ss >> str;

		return str;
	}

	LoginWindow::LoginWindow(size_t width, size_t height) : _wnd(width, height, "Login")
	{
		_width = width;
		_height = height;

		LoginWindowInit();

		_wnd.OnResized([this](EggAche::Window *, size_t x, size_t y)
		{
			_width = x;
			_height = y;
			LoginWindowInit();
		});

		_wnd.OnPress([this](EggAche::Window *, char ch)
		{
			if (ch == '\t')
			{
				if ((
					!_usernameInput->isActivated() &&
					!_pwdInput->isActivated()
					) || _pwdInput->isActivated())
				{
					_usernameInput->Activate(true);
					_pwdInput->Activate(false);
				}
				else
				{
					_usernameInput->Activate(false);
					_pwdInput->Activate(true);
				}
			}
			else if (ch != '\x08' && !isalnum(ch) && ch != '_')
				Prompt("Please Input Alpha/Number...");
			else
			{
				_usernameInput->InputKey(ch);
				_pwdInput->InputKey(ch);

				_username_before = _usernameInput->GetText();
				_pwd_before = _pwdInput->GetText();
			}

			_wnd.Refresh();
		});

		_wnd.OnClick([this](EggAche::Window *, size_t x, size_t y)
		{
			_usernameInput->Activate(false);
			_pwdInput->Activate(false);

			if (_usernameInput->TestClick(x, y))
				_usernameInput->Activate(true);
			if (_pwdInput->TestClick(x, y))
				_pwdInput->Activate(true);

			if (_loginBtn->TestClick(x, y) && _onLogin)
				_onLogin(_username_before, _pwd_before);

			if (_registerBtn->TestClick(x, y) && _onRegister)
				_onRegister(_username_before, _pwd_before);

			_wnd.Refresh();
		});
	}


	void LoginWindow::LoginWindowInit()
	{
		_bgCanvas = std::make_unique<EggAche::Canvas>(_width, _height);
		_wnd.SetBackground(_bgCanvas.get());
		auto i = _bgCanvas->DrawImg("Res/Loginbg.bmp", 0, 0, _width, _height);

		if (_prompt.get() != nullptr)
			(*_bgCanvas) -= _prompt.get();
		_prompt = std::make_unique<EggAche::Canvas>(
			_width / 2, _height / 2,
			_width * 15 / 20, _height * 1 / 10);
		(*_bgCanvas) += _prompt.get();

		Prompt("Welcome!!!");

		_loginBtn = std::make_unique<Button>(*_bgCanvas, "Login", _width * 15 / 20, _height * 4 / 10);
		_registerBtn = std::make_unique<Button>(*_bgCanvas, "Register", _width * 17 / 20, _height * 4 / 10);

		static const auto minInputWidth =
			_bgCanvas->GetTxtWidth("<Input your Password>");
		auto inputWidth = (_width * 1 / 5) > minInputWidth ? (_width * 1 / 5) : minInputWidth;

		_usernameInput = std::make_unique<Input>(
			*_bgCanvas, _username_before, "<Input your User ID>",
			(_width - inputWidth) * 37 / 40, _height * 2 / 10, inputWidth);
		_pwdInput = std::make_unique<Input>(
			*_bgCanvas, _pwd_before, "<Input your Password>",
			(_width - inputWidth) * 37 / 40, _height * 3 / 10, inputWidth);

		_wnd.Refresh();

	}

	UserInfoWindow::UserInfoWindow(size_t width, size_t height) : _wnd(width, height, "User Infomation")
	{
		_width = width;
		_height = height;

		UserInfoWindowInit();

		_wnd.OnResized([this](EggAche::Window *, size_t x, size_t y)
		{
			_width = x;
			_height = y;
			UserInfoWindowInit();
		});

		_wnd.OnClick([this](EggAche::Window *, size_t x, size_t y)
		{
			if (_pveBtn->TestClick(x, y) && _onPveBtn)
				if (_onPveBtn())
					_wnd.Refresh();

			if (_onlineUserBtn->TestClick(x, y) && _onOnlineUserBtn) 
				if (_onOnlineUserBtn()) {
					_wnd.Refresh();
				}
		});
	}

	void UserInfoWindow::UserInfoWindowInit()
	{
		_bgCanvas = std::make_unique<EggAche::Canvas>(_width, _height);
		_wnd.SetBackground(_bgCanvas.get());
		_bgCanvas->DrawImg("Res/Infobg.bmp", 0, 0, _width, _height);
		_bgCanvas->SetFont(18, "Consolas", 255, 255, 255);

		_onlineUserBtn = std::make_unique<Button>(*_bgCanvas, "Online User", _width * 17 / 20, _height * 7 / 10);
		_bgCanvas->DrawImgMask("Res/pve.bmp", "Res/pvemask.bmp", 35, 30, _width *  33/ 40, _height * 8 / 10 - 5, 0, 0, 0, 0);
		_pveBtn = std::make_unique<Button>(*_bgCanvas, "Player VS Enviroment", _width * 17 / 20, _height * 8 / 10);
		_bgCanvas->DrawTxt(_width * 1 / 20, _height * 1 / 20, std::string{ "Name: " + Driver::user.GetUerName() }.c_str());
		_bgCanvas->DrawImgMask("Res/userLogin.bmp", "Res/userLoginmask.bmp", 35, 30, _width * 1 / 40, _height * 1 / 20 - 5, 0, 0, 0, 0);
		auto i = Driver::user.GetStatus();

		switch (i)
		{
		case Player::Status::Gold:
			_bgCanvas->DrawImgMask("Res/Gold.bmp", "Res/Goldmask.bmp", 35, 30, _width * 1 / 40, _height * 2 / 20 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 1 / 20, _height * 2 / 20, "Status: Gold");
			break;
		case Player::Status::Silver:
			_bgCanvas->DrawImgMask("Res/Silver.bmp", "Res/Silvermask.bmp", 35, 30, _width * 1 / 40, _height * 2 / 20 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 1 / 20, _height * 2 / 20, "Status: Silver");
			break;
		case Player::Status::Bronze:
			_bgCanvas->DrawImgMask("Res/Bronze.bmp", "Res/Bronzemask.bmp", 35, 30, _width * 1 / 40, _height * 2 / 20 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 1 / 20, _height * 2 / 20, "Status: Bronze");
			break;
		default:
			break;
		}

		_bgCanvas->DrawImgMask("Res/monsternum.bmp", "Res/monsternummask.bmp", 35, 30, _width * 1 / 40, _height * 3 / 20 - 5, 0, 0, 0, 0);
		_bgCanvas->DrawTxt(_width * 1 / 20, _height * 3 / 20, std::string{ "Monster Num: " + std::to_string(Driver::user.GetMonsterNum()) }.c_str());
		_bgCanvas->DrawImgMask("Res/Win.bmp", "Res/Winmask.bmp", 35, 30, _width * 1 / 40, _height * 4 / 20 - 5, 0, 0, 0, 0);
		_bgCanvas->DrawTxt(_width * 1 / 20, _height * 4 / 20, std::string{ "Win Num: " + std::to_string(Driver::user.GetWinNum()) }.c_str());
		_bgCanvas->DrawImgMask("Res/defeat.bmp", "Res/defeatmask.bmp", 35, 30, _width * 1 / 40, _height * 5 / 20 - 5, 0, 0, 0, 0);
		_bgCanvas->DrawTxt(_width * 1 / 20, _height * 5 / 20, std::string{ "Failure Num: " + std::to_string(Driver::user.GetFailureNum()) }.c_str());
		_bgCanvas->DrawImgMask("Res/winrate.bmp", "Res/winratemask.bmp", 35, 30, _width * 1 / 40, _height * 6 / 20 - 5, 0, 0, 0, 0);
		auto a = Driver::user.GetWinRate();
		_bgCanvas->DrawTxt(_width * 1 / 20, _height * 6 / 20, std::string{ "Win Rate:" + std::to_string(Driver::user.GetWinRate()) }.c_str());

		_wnd.Refresh();
	}

	SelectMonsterWindow::SelectMonsterWindow(size_t width, size_t height) : _wnd(width, height, "Select A Monster for War")
	{
		_width = width;
		_height = height;

		SelectMonsterWindowInit();

		_wnd.OnResized([this](EggAche::Window *, size_t x, size_t y)
		{
			_width = x;
			_height = y;
			SelectMonsterWindowInit();
		});

		_wnd.OnClick([this](EggAche::Window *, size_t x, size_t y)
		{
			int i;

			for (i = 0; i != Driver::user.GetMonsterNum(); ++i) {
				if (_monsterVecBtn.at(i)->TestClick(x, y) && _onMonsterVecBtn) {
					_onMonsterVecBtn(i);
					break;
				}
			}

			if (_returnBtn->TestClick(x, y) && _onReturnBtn)
				if(_onReturnBtn())
					_wnd.Refresh();
		});
	}

	void SelectMonsterWindow::SelectMonsterWindowInit()
	{
		_bgCanvas = std::make_unique<EggAche::Canvas>(_width, _height);
		_wnd.SetBackground(_bgCanvas.get());
		_bgCanvas->DrawImg("Res/selectmonsterbg.bmp", 0, 0, _width, _height);
		_bgCanvas->SetFont(18, "Consolas", 255, 255, 255);
		_returnBtn = std::make_unique<Button>(*_bgCanvas, "Return", _width * 18 / 20, _height * 9 / 10);
		_bgCanvas->DrawImgMask("Res/phyle.bmp", "Res/phylemask.bmp", 35, 30, _width * 2 / 10, _height * 1/40 - 5, 0, 0, 0, 0);

		int cur = 0;
		for (auto i : Driver::user.GetMonsterVec()) {
			_monsterVecBtn.push_back(std::make_unique<Button>(*_bgCanvas, i->GetName().c_str(), _width * 1 / 10, _height * (2 * cur + 1) / 20));
			auto p = i->GetPhyle();
			switch (p)
			{
			case Monster::Phyle::Strength:
				_bgCanvas->DrawTxt(_width * 2 / 10, _height * (2 * cur + 1) / 20, "Phyle:Strength");
				break;
			case Monster::Phyle::Blood:
				_bgCanvas->DrawTxt(_width * 2 / 10, _height * (2 * cur + 1) / 20, "Phyle:Blood");
				break;
			case Monster::Phyle::Defense:
				_bgCanvas->DrawTxt(_width * 2 / 10, _height * (2 * cur + 1) / 20, "Phyle:Defense");
				break;
			case Monster::Phyle::Agile:
				_bgCanvas->DrawTxt(_width * 2 / 10, _height * (2 * cur + 1) / 20, "Phyle:Agile");
				break;
			default:
				break;
			}

			_bgCanvas->DrawImgMask("Res/strength.bmp", "Res/strengthmask.bmp", 35, 30, _width * 3 / 10, _height * 1 / 40 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 3 / 10, _height * (2 * cur + 1) / 20, std::string{ "Strength:" + std::to_string(i->GetStrength()) }.c_str());
			_bgCanvas->DrawImgMask("Res/blood.bmp", "Res/bloodmask.bmp", 35, 30, _width * 4 / 10, _height * 1 / 40 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 4 / 10, _height * (2 * cur + 1) / 20, std::string{ "Blood:" + std::to_string(i->GetBlood()) }.c_str());
			_bgCanvas->DrawImgMask("Res/defense.bmp", "Res/defensemask.bmp", 35, 30, _width * 5 / 10, _height * 1 / 40 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 5 / 10, _height * (2 * cur + 1) / 20, std::string{ "Defense:" + std::to_string(i->GetDefense()) }.c_str());
			_bgCanvas->DrawImgMask("Res/agile.bmp", "Res/agilemask.bmp", 35, 30, _width * 6 / 10, _height * 1 / 40 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 6 / 10, _height * (2 * cur + 1) / 20, std::string{ "Agile:" + std::to_string(i->GetAgile()) }.c_str());
			_bgCanvas->DrawImgMask("Res/point.bmp", "Res/pointmask.bmp", 35, 30, _width * 7 / 10, _height * 1 / 40 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 7 / 10, _height * (2 * cur + 1) / 20, std::string{ "attribute:" + std::to_string(i->GetPoint()) }.c_str());
			_bgCanvas->DrawImgMask("Res/exp.bmp", "Res/expmask.bmp", 35, 30, _width * 8 / 10, _height * 1 / 40 - 5, 0, 0, 0, 0);
			_bgCanvas->DrawTxt(_width * 8 / 10, _height * (2 * cur + 1) / 20, std::string{ "Exp:" + std::to_string(i->GetExp()) }.c_str());
			++cur;
		}

		_wnd.Refresh();
	}

	WarWindow::WarWindow(size_t width, size_t height) : _wnd(width, height, "War Window")
	{
		_width = width;
		_height = height;
		_ServerMosnterID = Driver::ServerWarMonster->GetMonsterID();
		_ClientMonsterID = Driver::ClientWarMonster->GetMonsterID();

		WarWindowInit();

		_wnd.OnResized([this](EggAche::Window *, size_t x, size_t y)
		{
			_width = x;
			_height = y;
			WarWindowInit();
		});

		_wnd.OnClick([this](EggAche::Window *, size_t x, size_t y)
		{
			int i;
			for (i = 0; i != Driver::ClientWarMonster->GetSkill().size(); ++i) {
					if (_skillBtn.at(i)->TestClick(x, y) && _onSkillBtn) {
						_onSkillBtn(i);
						break;
					}
			}

				if (_readyBtn->TestClick(x, y) && _onReadyBtn)
					if (_onReadyBtn())
						;
			

			
				if (_attackBtn->TestClick(x, y) && _onAttackBtn)
					if (_onAttackBtn())
						;
			

			_wnd.Refresh();
		});
	}
	void WarWindow::WarWindowInit()
	{
		_bgCanvas = std::make_unique<EggAche::Canvas>(_width, _height);
		_wnd.SetBackground(_bgCanvas.get());
		_bgCanvas->DrawImg("Res/Warbg.bmp", 0, 0, _width, _height);
		_bgCanvas->SetFont(18, "Consolas", 0, 0, 0);
		
		
		// prompt
		if (_prompt_serverblood.get() != nullptr)
			(*_bgCanvas) -= _prompt_serverblood.get();
		_prompt_serverblood = std::make_unique<EggAche::Canvas>(_width / 2, _height / 2, _width * 1 / 20, _height * 3 / 10);
		(*_bgCanvas) += _prompt_serverblood.get();
		PromptServerBlood(std::to_string(Driver::ServerWarMonster->GetCurBlood()));

		if (_prompt_serverenergy.get() != nullptr)
			(*_bgCanvas) -= _prompt_serverenergy.get();
		_prompt_serverenergy = std::make_unique<EggAche::Canvas>(_width / 2, _height / 2, _width * 1 / 20, _height * 7 / 20);
		(*_bgCanvas) += _prompt_serverenergy.get();
		PromptServerEnergy(std::to_string(Driver::ServerWarMonster->GetEnergy()));

		if (_prompt_clientblood.get() != nullptr)
			(*_bgCanvas) -= _prompt_clientblood.get();
		_prompt_clientblood = std::make_unique<EggAche::Canvas>(_width / 2, _height / 2, _width * 15 / 20, _height * 3 / 10);
		(*_bgCanvas) += _prompt_clientblood.get();
		PromptClientBlood(std::to_string(Driver::ClientWarMonster->GetCurBlood()));

		if (_prompt_clientenergy.get() != nullptr)
			(*_bgCanvas) -= _prompt_clientenergy.get();
		_prompt_clientenergy = std::make_unique<EggAche::Canvas>(_width / 2, _height / 2, _width * 15 / 20, _height * 7 / 20);
		(*_bgCanvas) += _prompt_clientenergy.get();
		PromptClientEnergy(std::to_string(Driver::ClientWarMonster->GetEnergy()));

		//init animation
		if (_rightToLeftAnimation.get() != nullptr)
			(*_bgCanvas) -= _rightToLeftAnimation.get();
		_rightToLeftAnimation = std::make_unique<EggAche::Canvas>(_width, _height, 0, 0);
		(*_bgCanvas) += _rightToLeftAnimation.get();

		if (_leftToRightAnimation.get() != nullptr)
			(*_bgCanvas) -= _leftToRightAnimation.get();
		_leftToRightAnimation = std::make_unique<EggAche::Canvas>(_width, _height, 0, 0);
		(*_bgCanvas) += _leftToRightAnimation.get();

		// -draw server monster
		_DrawMonster(_bgCanvas, _ServerMosnterID, _width * 1 / 20 - 20, _height * 9 / 20);
		_DrawMonster(_bgCanvas, _ClientMonsterID, _width * 15 / 20 - 20, _height * 9 / 20);

		_readyBtn = std::make_unique<Button>(*_bgCanvas, "Ready", _width * 18 / 20, _height * 9 / 10);
		_attackBtn = std::make_unique<Button>(*_bgCanvas, "Attack", _width * 17 / 20, _height * 1 / 20);
		for (auto i = 0; i != Driver::ClientWarMonster->GetSkill().size(); ++i) {
			_skillBtn.push_back(std::make_unique<Button>(*_bgCanvas, Driver::ClientWarMonster->GetSkill().at(i).c_str(), _width * 17 / 20, _height * (i + 2) / 20));
		}


		_wnd.Refresh();
	}

	void WarWindow::RightTOLeft()
	{
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 13 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 14 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 15 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_rightToLeftAnimation->Clear();
		_wnd.Refresh();

		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 10 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 11 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 12 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_rightToLeftAnimation->Clear();
		_wnd.Refresh();

		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 7 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 8 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 9 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_rightToLeftAnimation->Clear();
		_wnd.Refresh();

		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 4 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 5 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_rightToLeftAnimation->DrawImgMask("Res/attackRTOL.bmp", "Res/attackmaskRTOL.bmp", 140, 120, _width * 6 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_rightToLeftAnimation->Clear();
		_wnd.Refresh();
	}

	void WarWindow::LeftTORight()
	{
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 4 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 5 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 6 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_leftToRightAnimation->Clear();
		_wnd.Refresh();

		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 7 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 8 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 9 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_leftToRightAnimation->Clear();
		_wnd.Refresh();

		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 10 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 11 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 12 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_leftToRightAnimation->Clear();
		_wnd.Refresh();

		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 13 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 14 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_leftToRightAnimation->DrawImgMask("Res/attackLTOR.bmp", "Res/attackmaskLTOR.bmp", 140, 120, _width * 15 / 20 - 20, _height * 10 / 20, 0, 0, 0, 0);
		_wnd.Refresh();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		_leftToRightAnimation->Clear();
		_wnd.Refresh();
	}

	void WarWindow::_DrawMonster(std::unique_ptr<EggAche::Canvas>& bg, int id, size_t x_pos, size_t y_pos)
	{
		switch (id)
		{
		case 1:
			_bgCanvas->DrawImgMask("Res/misaka.bmp", "Res/misakamask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 2:
			_bgCanvas->DrawImgMask("Res/saten.bmp", "Res/satenmask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 3:
			_bgCanvas->DrawImgMask("Res/shirai.bmp", "Res/shiraimask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 4:
			_bgCanvas->DrawImgMask("Res/uiharu.bmp", "Res/uiharumask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 5:
			_bgCanvas->DrawImgMask("Res/saber.bmp", "Res/sabermask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 6:
			_bgCanvas->DrawImgMask("Res/emiya.bmp", "Res/emiyamask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 7:
			_bgCanvas->DrawImgMask("Res/gilgamesh.bmp", "Res/gilgameshmask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 8:
			_bgCanvas->DrawImgMask("Res/eastmoon.bmp", "Res/eastmoonmask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		case 9:
			_bgCanvas->DrawImgMask("Res/honghong.bmp", "Res/honghongmask.bmp", 280, 240, x_pos, y_pos, 0, 0, 0, 0);
			break;
		default:
			break;
		}
		
		
	}

	WinSettleAccountWindow::WinSettleAccountWindow(size_t width, size_t height) : _wnd(width, height, "Win!!!")
	{
		_width = width;
		_height = height;
		_monsternum = Driver::user.GetMonsterNum();
		_roomnum = Driver::RoomNum;
		_Username = Driver::user.GetUerName();

		WinSettleAccountWindowInit();

		_wnd.OnResized([this](EggAche::Window *, size_t x, size_t y)
		{
			_width = x;
			_height = y;
			WinSettleAccountWindowInit();
		});

		_wnd.OnClick([this](EggAche::Window *, size_t x, size_t y)
		{
			if (_returnBtn->TestClick(x, y) && _onReturnBtn)
				if (_onReturnBtn())
				{
					_wnd.Refresh();
				}
		});

	}

	void WinSettleAccountWindow::WinSettleAccountWindowInit()
	{
		_bgCanvas = std::make_unique<EggAche::Canvas>(_width, _height);
		_wnd.SetBackground(_bgCanvas.get());
		_bgCanvas->DrawImg("Res/settleAccountbg.bmp", 0, 0, _width, _height);
		_bgCanvas->SetFont(18, "Consolas", 255, 255, 255);

		if (Driver::IsWin && Driver::user.GetMonsterNum() < 9) {
			_returnBtn = std::make_unique<Button>(*_bgCanvas, "Return", _width * 9 / 20, _height * 8 / 10);
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 3 / 20, "Congratulations!!!");
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 4 / 20, "You will get a new monster, whose attributes are below!");
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 5 / 20, std::string{ "Name: " + Driver::ServerWarMonster->GetName() }.c_str());

			auto p = Driver::ServerWarMonster->GetPhyle();
			switch (p)
			{
			case Monster::Phyle::Strength:
				_bgCanvas->DrawTxt(_width * 9 / 20, _height * 6 / 20, "Phyle:Strength");
				break;
			case Monster::Phyle::Blood:
				_bgCanvas->DrawTxt(_width * 9 / 20, _height * 6 / 20, "Phyle:Blood");
				break;
			case Monster::Phyle::Defense:
				_bgCanvas->DrawTxt(_width * 9 / 20, _height * 6 / 20, "Phyle:Defense");
				break;
			case Monster::Phyle::Agile:
				_bgCanvas->DrawTxt(_width * 9 / 20, _height * 6 / 20, "Phyle:Agile");
				break;
			default:
				break;
			}

			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 7 / 20, std::string{ "Strength: " + std::to_string(Driver::ServerWarMonster->GetStrength()) }.c_str());
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 8 / 20, std::string{ "Blood: " + std::to_string(Driver::ServerWarMonster->GetBlood()) }.c_str());
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 9 / 20, std::string{ "Defense: " + std::to_string(Driver::ServerWarMonster->GetDefense()) }.c_str());
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 10 / 20, std::string{ "Agile: " + std::to_string(Driver::ServerWarMonster->GetAgile()) }.c_str());
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 11 / 20, "Skill: ");
			int i;
			for (i = 0; i != Driver::ServerWarMonster->GetSkill().size(); ++i) {
				_bgCanvas->DrawTxt(_width * (11 + i) / 20, _height * 11 / 20, Driver::ServerWarMonster->GetSkill().at(i).c_str());
			}
		}
		else if (Driver::IsWin && Driver::user.GetMonsterNum() >= 9) {
			_returnBtn = std::make_unique<Button>(*_bgCanvas, "Return", _width * 9 / 20, _height * 8 / 10);
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 3 / 20, "Congratulations!!!");
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 4 / 20, "You already have 9 monsters!");
		}
		

		_wnd.Refresh();
	}

	FailSettleAccountWindow::FailSettleAccountWindow(size_t width, size_t height) : _wnd(width, height, "Defeat!!!")
	{
		_width = width;
		_height = height;
		_roomnum = Driver::RoomNum;
		_username = Driver::user.GetUerName();

		FailSettleAccountWindowInit();

		_wnd.OnResized([this](EggAche::Window *, size_t x, size_t y)
		{
			_width = x;
			_height = y;
			FailSettleAccountWindowInit();
		});

		_wnd.OnClick([this](EggAche::Window *, size_t x, size_t y)
		{
			int i = 0;
			for (i = 0; i != Driver::user.GetMonsterNum(); ++i) {
				if (_monsterVecBtn.at(i)->TestClick(x, y) && _onMonsterVecBtn) {
					_onMonsterVecBtn(i);
					break;
				}
			}
		});

	}
	void FailSettleAccountWindow::FailSettleAccountWindowInit()
	{
		_bgCanvas = std::make_unique<EggAche::Canvas>(_width, _height);
		_wnd.SetBackground(_bgCanvas.get());
		_bgCanvas->DrawImg("Res/settleAccountbg.bmp", 0, 0, _width, _height);
		_bgCanvas->SetFont(18, "Consolas", 255, 255, 255);

		if (!Driver::IsWin) {
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 3 / 20, "Defeat!!!");
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 4 / 20, "Please chose a monster to lose.");
			_bgCanvas->DrawTxt(_width * 9 / 20, _height * 5 / 20, "If the monster is your last one, then you will get 3 new monsters.");
			int cur = 0;
			for (auto i : Driver::user.GetMonsterVec()) {
				_monsterVecBtn.push_back(std::make_unique<Button>(*_bgCanvas, i->GetName().c_str(), _width * 1 / 10, _height * (cur + 6) / 20));
				auto p = i->GetPhyle();
				switch (p)
				{
				case Monster::Phyle::Strength:
					_bgCanvas->DrawTxt(_width * 2 / 10, _height * (cur + 6) / 20, "Phyle:Strength");
					break;
				case Monster::Phyle::Blood:
					_bgCanvas->DrawTxt(_width * 2 / 10, _height * (cur + 6) / 20, "Phyle:Blood");
					break;
				case Monster::Phyle::Defense:
					_bgCanvas->DrawTxt(_width * 2 / 10, _height * (cur + 6) / 20, "Phyle:Defense");
					break;
				case Monster::Phyle::Agile:
					_bgCanvas->DrawTxt(_width * 2 / 10, _height * (cur + 6) / 20, "Phyle:Agile");
					break;
				default:
					break;
				}

				_bgCanvas->DrawTxt(_width * 3 / 10, _height * (cur + 6) / 20, std::string{ "Strength:" + std::to_string(i->GetStrength()) }.c_str());
				_bgCanvas->DrawTxt(_width * 4 / 10, _height * (cur + 6) / 20, std::string{ "Blood:" + std::to_string(i->GetBlood()) }.c_str());
				_bgCanvas->DrawTxt(_width * 5 / 10, _height * (cur + 6) / 20, std::string{ "Defense:" + std::to_string(i->GetDefense()) }.c_str());
				_bgCanvas->DrawTxt(_width * 6 / 10, _height * (cur + 6) / 20, std::string{ "Agile:" + std::to_string(i->GetAgile()) }.c_str());
				_bgCanvas->DrawTxt(_width * 7 / 10, _height * (cur + 6) / 20, std::string{ "attribute:" + std::to_string(i->GetPoint()) }.c_str());
				_bgCanvas->DrawTxt(_width * 8 / 10, _height * (cur + 6) / 20, std::string{ "Exp:" + std::to_string(i->GetExp()) }.c_str());
				++cur;
			}
		}

		_wnd.Refresh();
	}

	OnlineUserWindow::OnlineUserWindow(size_t width, size_t height) : _wnd(width, height, "Online User Information")
	{
		_width = width;
		_height = height;

		OnlineUserWindowInit();

		_wnd.OnResized([this](EggAche::Window *, size_t x, size_t y)
		{
			_width = x;
			_height = y;
			OnlineUserWindowInit();
		});

		_wnd.OnClick([this](EggAche::Window *, size_t x, size_t y)
		{
			if (_returnBtn->TestClick(x, y) && _onReturnBtn)
				if (_onReturnBtn())
					_wnd.Refresh();
		});
	}

	void OnlineUserWindow::OnlineUserWindowInit()
	{
		_bgCanvas = std::make_unique<EggAche::Canvas>(_width, _height);
		_wnd.SetBackground(_bgCanvas.get());
		_bgCanvas->DrawImg("Res/Online.bmp", 0, 0, _width, _height);
		_bgCanvas->SetFont(18, "Consolas", 255, 255, 255);

		_returnBtn = std::make_unique<Button>(*_bgCanvas, "Return", _width * 18 / 20, _height * 9 / 10);

		int index = Driver::OnlineUserInfo.find_first_of('|');
		std::string OnlineUserNUM = Driver::OnlineUserInfo.substr(0, index);
		auto s = Driver::OnlineUserInfo.substr(index + 1, Driver::OnlineUserInfo.length() - 1);
		for (int i = 0; i != std::stoi(OnlineUserNUM); ++i) {
			index = s.find_first_of('|');
			std::string n = s.substr(0, index);
			_bgCanvas->DrawTxt(_width / 20, _height * (i + 1) / 20, std::string{"Username:" + n}.c_str());
			s = s.substr(index + 1, s.length() - index - 1);
			index = s.find_first_of('|');
			std::string m = s.substr(0, index);
			int cur = 4;
			for (auto ch : m) {
				if (ch == '1') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Misaka");
				}
				else if (ch == '2') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Saten");
				}
				else if (ch == '3') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Shirai");
				}
				else if (ch == '4') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Uiharu");
				}
				else if (ch == '5') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Saber");
				}
				else if (ch == '6') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Emiya");
				}
				else if (ch == '7') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Gilgamesh");
				}
				else if (ch == '8') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "Eastmoon");
				}
				else if (ch == '9') {
					_bgCanvas->DrawTxt(_width * cur / 20, _height * (i + 1) / 20, "HongHong");
				}
				cur += 2;
			}
			s = s.substr(index + 1, s.length() - index - 1);
		}

		_wnd.Refresh();
	}
}

