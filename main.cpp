
/*   Dont know what do these do?
#define PLATFORM_DESKTOP
#define RAYLIB_WIN32
#define _CRT_SECURE_NO_WARNINGS
*/

/* Dont need this anymore cuz i've configured in
the project properties(linker/system, linker/advanced)

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
*/

#include <iostream>
#include <raylib.h>

using namespace std;

Color Green = Color{ 38, 185, 154, 255 };
Color Dark_Green = Color{ 20, 160, 133, 255 };
Color Light_Green = Color{ 129, 204, 184, 255 };
Color Yellow = Color{ 243, 213, 91, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
	float x, y;
	int speed_x, speed_y;
	int r;

	void Draw()
	{
		DrawCircle(x, y, r, Yellow);
	}

	void Update()
	{
		x += speed_x;
		y += speed_y;

		if ( (y + r >= GetScreenHeight()) || (y - r <= 0) )
		{
			speed_y *= -1;
		}

		if ((x + r >= GetScreenWidth()))
		{
			cpu_score++;
			ResetBall();
		}
		
		if (x - r <= 0)
		{
			player_score++;
			ResetBall();
		}
	}

	void ResetBall()
	{
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speed_choices[2] = { -1, 1 };
		speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];
	}
};

class Paddle
{
public:
	float x, y, w, h;
	int speed;

	void Draw()
	{
		DrawRectangleRounded(Rectangle{ x, y, w, h}, 0.8, 0, WHITE);
	}

	void Update()
	{
		y = GetMouseY();

		LimitMovement();
	}
protected:
	void LimitMovement()
	{
		if (y <= 0)
		{
			y = 0;
		}

		if (y + h >= GetScreenHeight())
		{
			y = GetScreenHeight() - h;
		}
	};
};

class CPUPaddle : public Paddle
{
public:
	void Update(int ball_y)
	{
		if (y + h / 2 > ball_y)
		{
			y = y - speed;
		}

		if (y + h / 2 <= ball_y)
		{
			y += speed;
		}

		LimitMovement();
	}
};

Ball ball;
Paddle player;
CPUPaddle cpu;

int main()
{
	cout << "Starting the game .." << endl;
	
	const int screenW = 1280;
	const int screenH = 800;

	ball.x = screenW / 2;
	ball.y = screenH / 2;
	ball.r = 20;
	ball.speed_x = 7;
	ball.speed_y = 7;

	player.w = 25;
	player.h = 120;
	player.x = screenW - player.w - 10;
	player.y = screenH / 2 - player.h / 2;
	player.speed = 6;

	cpu.h = 120;
	cpu.w = 25;
	cpu.x = 10;
	cpu.y = screenH / 2 - cpu.h;
	cpu.speed = 6;

	InitWindow(screenW, screenH, "RaylibxC++ Pong Game");
	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		// Update
		ball.Update();
		player.Update();
		cpu.Update(ball.y);

		// Check for collisions
		if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.r, Rectangle{player.x, player.y, player.w, player.h}))
		{
			ball.speed_x *= -1;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.r, Rectangle{ cpu.x, cpu.y, cpu.w, cpu.h }))
		{
			ball.speed_x *= -1;
		}

		// Draw
		ClearBackground(Dark_Green);
		DrawRectangle(screenW / 2, 0, screenW / 2, screenH, Green);
		DrawCircle(screenW / 2, screenH / 2, 150, Light_Green);
		DrawLine(screenW / 2, 0, screenW / 2, screenH, WHITE);

		ball.Draw();
		cpu.Draw();
		player.Draw();

		DrawText(TextFormat("%i", cpu_score), screenW / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score), screenW * 3 / 4 - 20, 20, 80, WHITE);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}