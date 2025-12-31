// 这个程序在控制台里用字符模拟跨年烟花动画

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <thread>
#ifdef _WIN32
#include <windows.h>
#endif

// 这个代码块负责把控制台编码设置为 utf-8
void init_console_utf8()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

// 这个代码块负责清屏
void clear_screen()
{
    std::cout << "\x1b[2J\x1b[H";
}

// 这个代码块负责暂停一定毫秒
void sleep_ms(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// 这个代码块负责打印一帧字符画面（支持彩色烟花）
void draw_frame(const std::vector<std::string> &screen)
{
    clear_screen();

    for (const auto &line : screen)
    {
        for (char ch : line)
        {
            if (ch == 'r')
            {
                std::cout << "\x1b[31m*\x1b[0m";
            }
            else if (ch == 'g')
            {
                std::cout << "\x1b[32m*\x1b[0m";
            }
            else if (ch == 'y')
            {
                std::cout << "\x1b[33m*\x1b[0m";
            }
            else if (ch == 'b')
            {
                std::cout << "\x1b[34m*\x1b[0m";
            }
            else if (ch == 'm')
            {
                std::cout << "\x1b[35m*\x1b[0m";
            }
            else if (ch == 'c')
            {
                std::cout << "\x1b[36m*\x1b[0m";
            }
            else
            {
                std::cout << ch;
            }
        }
        std::cout << '\n';
    }
}

// 这个代码块负责生成一个空白画面
std::vector<std::string> make_empty_screen(int width, int height)
{
    std::vector<std::string> screen(height, std::string(width, ' '));
    return screen;
}

// 这个代码块负责在画面上画一颗星星背景
void draw_stars(std::vector<std::string> &screen, int count)
{
    int height = static_cast<int>(screen.size());
    int width = static_cast<int>(screen[0].size());
    for (int i = 0; i < count; i++)
    {
        int x = std::rand() % width;
        int y = std::rand() % height;
        screen[y][x] = '.';
    }
}

// 这个代码块负责在画面中央显示一行文字
void draw_center_text(std::vector<std::string> &screen, const std::string &text)
{
    int height = static_cast<int>(screen.size());
    int width = static_cast<int>(screen[0].size());
    int y = height / 2;
    int x = (width - static_cast<int>(text.size())) / 2;
    if (x < 0)
    {
        x = 0;
    }
    for (std::size_t i = 0; i < text.size() && x + static_cast<int>(i) < width; i++)
    {
        screen[y][x + static_cast<int>(i)] = text[i];
    }
}

// 这个代码块负责倒计时场景
void scene_countdown(int width, int height)
{
    for (int n = 5; n >= 1; n--)
    {
        for (int k = 0; k < 3; k++)
        {
            std::vector<std::string> screen = make_empty_screen(width, height);
            draw_stars(screen, 80);
            std::string text = std::to_string(n);
            draw_center_text(screen, text);
            draw_frame(screen);
            sleep_ms(400);
        }
    }
}

// 这个代码块负责单个烟花上升和爆炸
void scene_firework(int width, int height, int center_x, const std::string &message)
{
    int ground_y = height - 2;

    // 火球上升
    for (int step = 0; step < 10; step++)
    {
        std::vector<std::string> screen = make_empty_screen(width, height);
        draw_stars(screen, 60);
        int y = ground_y - step * 2;
        if (y < 2)
        {
            y = 2;
        }
        if (center_x >= 0 && center_x < width && y >= 0 && y < height)
        {
            screen[y][center_x] = '|';
        }
        draw_frame(screen);
        sleep_ms(80);
    }

    int mid_y = height / 3;

    // 粒子爆炸
    // 爆炸时发出一声提示音
#ifdef _WIN32
    Beep(900, 80);
    Beep(1300, 120);
#endif
    for (int frame = 0; frame < 20; frame++)
    {
        std::vector<std::string> screen = make_empty_screen(width, height);
        draw_stars(screen, 40);

        int particle_count = 120;
        float max_radius = static_cast<float>(frame) * 0.7f;
        if (max_radius < 1.0f)
        {
            max_radius = 1.0f;
        }

        for (int i = 0; i < particle_count; i++)
        {
            float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
            float radius = max_radius * (0.3f + static_cast<float>(std::rand() % 70) / 100.0f);
            int x = center_x + static_cast<int>(std::cos(angle) * radius * 2.0f);
            int y = mid_y + static_cast<int>(std::sin(angle) * radius);
            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                int color_pick = std::rand() % 6;
                char color_char = 'r';
                if (color_pick == 0) color_char = 'r';
                else if (color_pick == 1) color_char = 'g';
                else if (color_pick == 2) color_char = 'y';
                else if (color_pick == 3) color_char = 'b';
                else if (color_pick == 4) color_char = 'm';
                else color_char = 'c';
                screen[y][x] = color_char;
            }
        }

        if (frame > 6)
        {
            draw_center_text(screen, message);
        }

        draw_frame(screen);
        sleep_ms(90);
    }

    // 粒子结束后让文字停留
    for (int frame = 0; frame < 12; frame++)
    {
        std::vector<std::string> screen = make_empty_screen(width, height);
        draw_stars(screen, 30);
        draw_center_text(screen, message);
        draw_frame(screen);
        sleep_ms(120);
    }
}

// 这个代码块负责过渡场景
void scene_transition(int width, int height)
{
    for (int frame = 0; frame < 20; frame++)
    {
        std::vector<std::string> screen = make_empty_screen(width, height);
        int star_count = 20 + frame * 2;
        draw_stars(screen, star_count);
        draw_center_text(screen, "新年快乐！");
        draw_frame(screen);
        sleep_ms(120);
    }
}

// 这个代码块负责大结局场景（每一发都有上升和爆炸）
void scene_finale(int width, int height)
{
    int ground_y = height - 2;

    int firework_count = 8;
    for (int n = 0; n < firework_count; n++)
    {
        int center_x = 5 + std::rand() % (width - 10);
        int target_y = 3 + std::rand() % (height / 2);

        // 火球上升
        int step_count = (ground_y - target_y) / 2;
        if (step_count < 6)
        {
            step_count = 6;
        }
        for (int step = 0; step < step_count; step++)
        {
            int y = ground_y - step * 2;
            if (y < target_y)
            {
                y = target_y;
            }

            std::vector<std::string> screen = make_empty_screen(width, height);
            draw_stars(screen, 40);
            if (center_x >= 0 && center_x < width && y >= 0 && y < height)
            {
                screen[y][center_x] = '|';
            }
            
            draw_frame(screen);
            sleep_ms(60);
        }
        // 爆炸
        int particle_count = 80;
        // 爆炸时发出一声提示音
    #ifdef _WIN32
        Beep(900, 80);
        Beep(1300, 120);
    #endif

        for (int frame = 0; frame < 14; frame++)
        {
            std::vector<std::string> screen = make_empty_screen(width, height);
            draw_stars(screen, 30);

            float max_radius = 1.0f + static_cast<float>(frame) * 0.6f;
            for (int i = 0; i < particle_count; i++)
            {
                float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
                float radius = max_radius * (0.3f + static_cast<float>(std::rand() % 70) / 100.0f);
                int x = center_x + static_cast<int>(std::cos(angle) * radius * 2.0f);
                int y = target_y + static_cast<int>(std::sin(angle) * radius);
                if (x >= 0 && x < width && y >= 0 && y < height)
                {
                    int color_pick = std::rand() % 6;
                    char color_char = 'r';
                    if (color_pick == 0) color_char = 'r';
                    else if (color_pick == 1) color_char = 'g';
                    else if (color_pick == 2) color_char = 'y';
                    else if (color_pick == 3) color_char = 'b';
                    else if (color_pick == 4) color_char = 'm';
                    else color_char = 'c';
                    screen[y][x] = color_char;
                }
            }

            draw_frame(screen);
            sleep_ms(90);
        }
    }
}

int main()
{
    init_console_utf8();//解决该死的编码问题
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    int screen_width = 60;
    int screen_height = 22;

    // 依次播放所有场景
    scene_countdown(screen_width, screen_height);
    scene_firework(screen_width, screen_height, screen_width / 2, "再见，2025！");
    scene_transition(screen_width, screen_height);
    scene_firework(screen_width, screen_height, screen_width / 2, "你好，2026！");
    scene_finale(screen_width, screen_height);

    std::vector<std::string> screen = make_empty_screen(screen_width, screen_height);
    draw_center_text(screen, "按回车退出...");
    draw_frame(screen);
    std::cin.get();

    return 0;
}
