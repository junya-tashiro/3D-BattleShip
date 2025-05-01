# 3D-BattleShip

## macOS向け 3D_BattleShip ビルド環境構築手順

このプロジェクトは **Qt 5.15.2** および **OpenGL（GLUT/GLU）** を利用しています。  
macOS (Intel / Apple Silicon 両対応) でのビルド手順は以下の通りです。

---

### 0. 前提環境

- macOS 13 Ventura 以降
- [Homebrew](https://brew.sh/) がインストール済み
- **Xcode** および **Command Line Tools** がインストール済み

### 1. 必要ツールのインストール

```bash
# Homebrew が未インストールなら
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# XQuartz（GLUを含むOpenGL環境）
brew install --cask xquartz

# Xcode Command Line Tools（未インストールの場合）
xcode-select --install
```

### 2. Qt 5.15.2 のインストール
Qt公式インストーラでは Qt 5.15.2 が非表示なため, オフラインインストーラを使用します。
- 🔗 [Qt 5.15.2 オフラインインストーラ（ミラー等）](https://download.qt.io/archive/qt/5.15/5.15.2/)
- インストール時は以下を選択してください：
  - Qt 5.15.2 for macOS (clang_64)
  - インストール先は /Users/yourname/Qt など

### 3. 環境変数の設定(任意)
```bash
echo 'export PATH=$HOME/Qt/5.15.2/clang_64/bin:$PATH' >> ~/.zshrc
source ~/.zshrc
```

### 4. プロジェクトのビルド
```bash
# プロジェクトディレクトリへ
cd 3D_BattleShip

# Makefile生成（SDKバージョンチェックを無効化）
qmake 3D_BattleShip.pro CONFIG+=sdk_no_version_check

# ビルド
make
```

### 5. アプリの実行
```bash
open 3D_BattleShip.app
```