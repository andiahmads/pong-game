#include <cstdlib>
#include <iostream>
#include <raylib.h>
using namespace std;

const Color white = (Color){255, 255, 255, 255};
const Color black = (Color){0, 0, 0, 255};
int player_score = 0;
int cpu_score = 0;

Color Green = Color{38, 185, 154, 255};
Color Yellow = Color{253, 249, 0, 255};
Color Dark_Blue = Color{0, 82, 172, 255};

class Ball {
public:
  float x, y;           //-> koordinat bola
  int speed_x, speed_y; //-> kecepatan bola
  int radius;

  // mengambar bola
  void Draw() { DrawCircle(x, y, radius, Yellow); }

  // update posisi bola
  void Update() {
    x += speed_x;
    y += speed_y;

    // masalah berikutnya adalah, bola memantul keluar dari jendela
    // untuk mengatasi masalah ini, yang harus dilakukan adalah
    // 1. memeriksa apakah titik bawah bola > screen_height maka ubah arah pada
    // sumbu y
    // 2. jika tidak ball.y+

    // ika bagian atas atau bagian bawah lingkaran mencapai atau melewati batas
    // atas atau batas bawah layar, maka arah pergerakan lingkaran pada sumbu Y
    // harus diubah.
    if (y + radius >= GetScreenHeight() || y - radius <= 0) {
      // adi, jika speed_y memiliki nilai positif, maka setelah operasi ini,
      // nilainya akan menjadi negatif, dan sebaliknya. Ini adalah cara yang
      // umum digunakan untuk mengubah arah atau orientasi dari suatu nilai
      // numerik.
      speed_y *= -1; //-> speed_y *= -1,-2,-3,-4,-5-6,-7
    }

    // memerika tabrakan dari kiri dan kanan
    // cpu wins
    if (x + radius >= GetScreenWidth()) {
      cpu_score++;
      ResetBall();
    }

    // player wins
    if (x - radius <= 0) {
      player_score++;
      ResetBall();
    }
  }

  void ResetBall() {
    x = GetScreenWidth() / 2;
    y = GetScreenHeight() / 2;

    int speed_choice[2] = {-1, 1};
    //     speed_x *= speed_choice[GetRandomValue(0, 1)];: Ini mengambil nilai
    //     acak antara 0 dan 1 dengan menggunakan fungsi GetRandomValue(0, 1).
    //     Hasilnya digunakan sebagai indeks untuk memilih secara acak antara
    //     elemen -1 dan 1 dari array speed_choice. Nilai ini kemudian dikalikan
    //     dengan speed_x, menghasilkan arah kecepatan acak pada sumbu X.
    //
    // speed_y *= speed_choice[GetRandomValue(0, 1)];: Ini melakukan hal yang
    // sama dengan langkah sebelumnya, tetapi pada sumbu Y. Ini menghasilkan
    // arah kecepatan acak pada sumbu Y.
    speed_x *= speed_choice[GetRandomValue(0, 1)];
    speed_y *= speed_choice[GetRandomValue(0, 1)];
  }
};

class Paddle {
public:
  float x, y;
  float width, height;
  int speed;

  void Draw() {
    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
  }

  void Update() {

    if (IsKeyDown(KEY_UP)) {
      // Jika tombol panah atas ditekan, pindahkan objek ke atas dengan
      // kecepatan yang ditentukan oleh variabel speed."
      y = y - speed;
    }
    if (IsKeyDown(KEY_DOWN)) {
      y = y + speed;
    }

    LimitMovement();
  }

protected:
  // mengatasi rectangle keluar layar pada posisi atas
  void LimitMovement() {
    if (y <= 0) {
      y = 0;
    }

    // mengatasi rectangle keluar layar pada posisi bawah
    // apakah bagian bawah objek telah mencapai atau melampaui batas bawah
    // layar. Jika iya, artinya objek sedang di luar atau di batas bawah
    // layar.
    if (y + height >= GetScreenHeight()) {

      // Sebagai contoh, jika tinggi layar adalah 600 piksel dan tinggi objek
      // adalah 50 piksel, pernyataan tersebut akan menetapkan nilai y
      // sedemikian rupa sehingga objek tersebut ditempatkan di bagian bawah
      // layar pada koordinat 788 (800 - 120). Dengan cara ini, objek tidak
      // akan melampaui batas bawah layar.
      y = GetScreenHeight() - height;
    }
  }
};

class CpuPaddle : public Paddle {
public:
  // membuat AI dasar
  void Update(int ball_y) {
    // 1. apakah posisi bola y berada ditengahnya, berada diatas posisi y pusat
    // pemukul.
    // 2. jika ya -> gerakkan pemukul keatas, dan jika tidak gerakkan y kebawah.
    if (y + height / 2 > ball_y) {
      // pindahkan objek ke atas dengan kecepatan yang ditentukan oleh variabel
      // speed."
      y = y - speed;
    }

    // periksa apakah pusat bola y lebih kecil dari pusat dayung y
    if (y + height / 2 <= ball_y) {
      // pindahkan objek ke bawah dengan kecepatan yang ditentukan oleh variabel
      // speed."
      y = y + speed;
    }

    LimitMovement();
  }
};

// inisiasi class
Ball ball;
Paddle player;
CpuPaddle cpu;

Vector2 InitVectorFromBall(const Ball &ball) { return {ball.x, ball.y}; }

Rectangle InitRectangleFromPlayer(const Paddle &player) {
  return {player.x, player.y, player.width, player.height};
}

int main() {
  cout << "starting game..." << endl;
  // L = 1280
  // T = 800
  // x = bergerak -> kiri & kanan
  // y = bergerak -> atas & bawah
  // koordinat jendela = lebar/2 & tinggi/2 = 640*400

  const float screen_widht = 1280;
  const float screen_height = 800;

  InitWindow(screen_widht, screen_height, "Pong Game With C++");
  SetTargetFPS(60);

  // implement ball
  ball.radius = 20;
  ball.x = screen_widht / 2;
  ball.y = screen_height / 2;
  ball.speed_x = 7;
  ball.speed_y = 7;

  // implement player
  // lebar persegi = 25px
  // panjang = 10px
  // yPos = screen_widht-25-10px = 1245
  // kenapa 10 karena ingin offset 10 px dari ujung layar
  // mencari koordinat sumbu_y:
  // y_center = screen_height(800)/2 - rectangle_height(120)/2; = 360
  player.width = 25;
  player.height = 120;
  player.x = screen_widht - player.width - 10;
  player.y = screen_height / 2 - player.height / 2;
  player.speed = 6;

  // implement CPU
  cpu.width = 25;
  cpu.height = 120;
  cpu.x = 10;
  cpu.y = screen_height / 2 - cpu.height / 2;
  cpu.speed = 6;

  bool pause = 0;
  int frameCounter = 0;

  const char *username = std::getenv("USER");

  while (!WindowShouldClose()) {
    BeginDrawing();

    if (IsKeyPressed(KEY_SPACE)) {
      pause = !pause;
    }

    if (!pause) {
      player.Update();
      ball.Update();
      cpu.Update(ball.y);

      Vector2 vec = InitVectorFromBall(ball);
      Rectangle recPlayer = InitRectangleFromPlayer(player);
      Rectangle recCPU = InitRectangleFromPlayer(cpu);

      // check apakah bola menyentuh pemukul, ini menggunakan fungsi standar
      // dari raylib.
      if (CheckCollisionCircleRec(vec, ball.radius, recPlayer)) {
        ball.speed_x *= -1;
      }

      if (CheckCollisionCircleRec(vec, ball.radius, recCPU)) {
        ball.speed_x *= -1;
      }

      // karena bola meninggal kan jejak setiap kali terjadi event loop, maka
      // sebelum mengambar object lain, lakukan clear
      ClearBackground(Dark_Blue);

      DrawCircle(screen_widht / 2, screen_height / 2, 150, LIGHTGRAY);

      DrawLine(screen_widht / 2, 0, screen_widht / 2, screen_height, white);
      player.Draw();
      cpu.Draw();
      ball.Draw();

      // gambar score
      DrawText(TextFormat("CPU:%i", cpu_score), screen_widht / 4 - 20, 20, 30,
               white);
      DrawText(TextFormat("%s:%i", username, player_score),
               3 * screen_widht / 4 - 20, 20, 30, white);
      // on pause, we draw a blinking message
      DrawText("PRESS SPACE to PAUSE BALL MOVEMENT", 10, GetScreenHeight() - 25,
               20, white);

    } else
      frameCounter++;
    if (pause && ((frameCounter / 30) % 2))
      DrawText("PAUSE", screen_widht / 2 - player.height / 2, screen_height / 2,
               35, WHITE);
    DrawFPS(10, 10);

    EndDrawing();
  };

  CloseWindow();
  return 0;
}
