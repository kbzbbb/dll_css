#include "protract.h"

void protract::Get_data() {
	data::Game_handle = FindWindow(NULL, "Counter-Strike Source");
	if (data::Game_handle == 0)
		return;
	data::pid = GetCurrentProcessId();
	if (data::pid == 0)
		return;
	data::handle = OpenProcess(PROCESS_ALL_ACCESS, false, data::pid);
	if (data::handle == nullptr)
		return;
	data::module = (uintptr_t)GetModuleHandleA("server.dll");
	if (data::module == 0)
		return;
	data::module2 = (uintptr_t)GetModuleHandleA("engine.dll");
	if (data::module2 == 0)
		return;
}

void protract::getGameRect(RECT& RectGame)
{
	RECT stRect, stKhRect;
	GetWindowRect(data::Game_handle, &stRect);
	GetClientRect(data::Game_handle, &stKhRect);
	RectGame.left = stRect.left;
	RectGame.right = stRect.right;
	RectGame.top = stRect.bottom - stKhRect.bottom;
	RectGame.bottom = stRect.bottom;
}

void protract::protract_pane(float x, float y, float w, float h, ImColor color, float t)
{
	float cx = x + w;
	float cy = y + h;

	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(cx, y), color, t);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(cx, y), ImVec2(cx, cy), color, t);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(cx, cy), ImVec2(x, cy), color, t);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, cy), ImVec2(x, y), color, t);
}

void protract::cheatDraw()
{
	protract::Get_data();

	int Gamer_quantity = 0;
	uintptr_t list_head = data::module + 0x4F615C;
	uintptr_t w_head = 0x0;
	int W_camp = 0;
	uintptr_t repeat = 0x0;
	Figure_Data figure_data;
	Figure_Data W;
	uintptr_t Matrix_address;
	int ViewWidth = 0;
	int ViewHeight = 0;
	float Matrix[4][4];
	RECT RectGame;

	getGameRect(RectGame);

	ViewWidth = (RectGame.right - RectGame.left) / 2;
	ViewHeight = (RectGame.bottom - RectGame.top) / 2;

	Gamer_quantity = read_multilevel<int>(data::module, data::gamer_quantity);
	Matrix_address = read_multilevel<int>(data::module2, data::Matrix_address);
	w_head = read_multilevel<int>(data::module, data::w);
	W.camp = read_memory<int>(w_head + 0x1F4);
	W.x = read_memory<float>(w_head + 0x308);
	W.y = read_memory<float>(w_head + 0x30C);
	W.z = read_memory<float>(w_head + 0x310);

	for (int i = 1; i < Gamer_quantity; i++) {
		repeat = read_memory<int>(list_head + i * 0x10);
		figure_data.life = read_memory<int>(repeat + 0xe4);
		figure_data.camp = read_memory<int>(repeat + 0x1F4);
		if(figure_data.life == 1) continue;
		figure_data.x = read_memory<float>(repeat + 0x308);
		figure_data.y = read_memory<float>(repeat + 0x30C);
		figure_data.z = read_memory<float>(repeat + 0x310);

		read_list<float>(Matrix_address + 0x2D4, &Matrix, 4 * 4);

		float Sc_z;
		float Sc_x;
		float Sc_foot;
		float Sc_head;

		Sc_z = Matrix[2][0] * figure_data.x + Matrix[2][1] * figure_data.y + Matrix[2][2] * figure_data.z + Matrix[2][3];
		float Scale = 1 / Sc_z;
		if (Sc_z < 0) continue;
		Sc_x = ViewWidth + (Matrix[0][0] * figure_data.x + Matrix[0][1] * figure_data.y + Matrix[0][2] * figure_data.z + Matrix[0][3]) * Scale * ViewWidth;
		Sc_foot = ViewHeight - (Matrix[1][0] * figure_data.x + Matrix[1][1] * figure_data.y + Matrix[1][2] * (figure_data.z - 8) + Matrix[1][3]) * Scale * ViewHeight;
		Sc_head = ViewHeight - (Matrix[1][0] * figure_data.x + Matrix[1][1] * figure_data.y + Matrix[1][2] * (figure_data.z + 78) + Matrix[1][3]) * Scale * ViewHeight;

		float BoxHeight = Sc_foot - Sc_head;
		float BoxWidth = BoxHeight * 0.526515151552;
		Sc_x -= BoxWidth / 2;
		Sc_head -= 0.0F;

		if (data::round_range == true) {
			ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ViewWidth, ViewHeight), data::round, ImColor(255, 0, 0, 255), 0, 1.0f);
		}
		if (data::lock == true) {
			float distance = abs(sqrt(pow((Sc_x + BoxWidth / 2) - ViewWidth, 2) + pow(Sc_head - ViewHeight, 2)));
			if (distance < data::round) {
				if (figure_data.camp != W.camp) {
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(ViewWidth, ViewHeight), ImVec2(Sc_x + BoxWidth / 2, Sc_head), ImColor(0, 0, 255, 255), 1.0f);
				}
			}
		}
		if (data::Crosshairs == true) {
			ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ViewWidth, ViewHeight), 1.0f, ImColor(255, 0, 0, 255), 0, 1.0f);
		}

		if (figure_data.camp != W.camp) {
			if (data::line == true) {
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(ViewWidth, 0), ImVec2(Sc_x + BoxWidth / 2, Sc_head), ImColor(0, 0, 255, 255), 1.0f);
			}
			if (data::life == true) {
				float hpPercentage = figure_data.life / 100.0f;
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(Sc_x, Sc_head), ImVec2(Sc_x, Sc_head + BoxHeight * hpPercentage), ImColor(255, 0, 0, 255), 2.0f);
			}
			if (data::pane == true) {
				protract_pane(Sc_x, Sc_head, BoxWidth, BoxHeight, ImColor(0, 255, 0, 255), 1.0f);
			}
		}

		if (data::teammate == true) {
			if (figure_data.camp == W.camp) {
				if (data::line == true) {
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(ViewWidth, 0), ImVec2(Sc_x + BoxWidth / 2, Sc_head), ImColor(0, 0, 255, 255), 1.0f);
				}
				if (data::life == true) {
					float hpPercentage = figure_data.life / 100.0f;
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(Sc_x, Sc_head), ImVec2(Sc_x, Sc_head + BoxHeight * hpPercentage), ImColor(255, 0, 0, 255), 2.0f);
				}
				if (data::pane == true) {
					protract_pane(Sc_x, Sc_head, BoxWidth, BoxHeight, ImColor(255, 0, 255, 255), 1.0f);
				}
			}

		}
	}
}

void protract::UI()
{
	ImGui::Begin(u8"CS起源辅助");
	if (ImGui::CollapsingHeader(u8"绘制"))
	{
		ImGui::Checkbox(u8"绘制射线", &data::line);
		ImGui::SameLine();
		ImGui::Checkbox(u8"绘制血量", &data::life);
		ImGui::Checkbox(u8"绘制方框", &data::pane);
		ImGui::SameLine();
		ImGui::Checkbox(u8"绘制范围", &data::round_range);
		ImGui::Checkbox(u8"绘制锁定", &data::lock);
		ImGui::SameLine();
		ImGui::Checkbox(u8"绘制队友", &data::teammate);
		ImGui::Checkbox(u8"绘制准心", &data::Crosshairs);
	}
	if (ImGui::CollapsingHeader(u8"设置参数")) {
		ImGui::SliderFloat(u8"自瞄范围设置", &data::round, 0.0f, 500.0f);
	}
	ImGui::End();
}

