//SpresenseにSD差した状態でSDの中のファイルをシリアルに表示する

#include <Arduino.h>
#include <SDHCI.h>

SDClass SD;  // Spresense専用のSDクラス

// 再帰的にディレクトリを表示
void listDir(File dir, int depth) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;

    for (int i = 0; i < depth; ++i) Serial.print("  ");

    if (entry.isDirectory()) {
      Serial.print("[DIR] ");
      Serial.println(entry.name());
      listDir(entry, depth + 1);
    } else {
      Serial.print(entry.name());
      Serial.print("\t");
      Serial.print(entry.size());
      Serial.println(" bytes");
    }
    entry.close();
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) ;  // シリアル接続待ち
  Serial.println("SDカードの初期化を開始...");

  if (!SD.begin()) {
    Serial.println("SDカードを初期化できません。挿入状態とフォーマットを確認してください。");
    return;
  }

  Serial.println("SDカード初期化成功。ルートディレクトリ:");
  File root = SD.open("/");
  if (!root) {
    Serial.println("ルートディレクトリを開けません。");
    return;
  }

  listDir(root, 0);
  root.close();
  Serial.println("一覧表示完了。");
}

void loop() {
  delay(1000);
}
