This is a fork of ChrisAnd1998s Version of the Anno1800ModMenu (https://github.com/ChrisAnd1998/Anno1800ModMenu/tree/main). All rights belong to him.

## Linux Fix:
I will NOT maintain this fix further.

On Linux, loading the mod DLLs does not work in the same way as on Windows systems.
This fix should solve this problem.
I tested it on my Linux Mint system with Anno 1800 started from Lutris, as I don't have the Steam version of the game. In theory, it should also work for the Steam version, but I can't test it there.
I have successfully tested it with Proton Hotfix, -Experimental and -GE 11.07. Everytime with DX11.

If it does not work, feel free to use the fix as a basis for your own solution.

The code for this mod fix was created by AI and briefly reviewed by me.

## Usage:

1. Copie the original Mod Menu DLLs into `Anno 1800/Bin/Win64`
2. Download and extract the fix into `Anno 1800/Bin/Win64`
3. Execute the `install.sh` script (The script just copies necessary proton files into the anno directory.):
```bash
sudo chmod +x ./install.sh
./install.sh \
  --game-dir "/path/to/Anno 1800/Bin/Win64" \
  --proton "$HOME/.steam/steam/steamapps/common/Proton Hotfix"
```
4. Don't forget to set the dll Overrides for Proton. (`WINEDLLOVERRIDES="version=n,b" %command%`)


# Anno1800ModMenu (origianl Readme)
A mod menu for Anno 1800 based on Dear ImGui. You can show/hide the menu with F7.


![Screenshot 2023-01-06 162818](https://user-images.githubusercontent.com/50437199/211046155-be8269ec-820a-418a-8d0b-5cf994484c90.png)
![Screenshot 2023-01-06 162905](https://user-images.githubusercontent.com/50437199/211046187-76f9d6c5-25ac-41aa-8b10-3ce1375b77fe.png)


# Installation
Extract Anno1800ModMenuDX11.dll, Anno1800ModMenuDX12.dll and TextShaping.dll to STEAMAPPS\common\Anno 1800\Bin\Win64. Then just run the game.
To show the menu you can hit F7 when you are at main menu or in game.

<img width="924" alt="213380594-38f23048-800f-4a1a-a9a4-6a909d7faf51" src="https://user-images.githubusercontent.com/50437199/230776612-88154d60-984b-46f8-bf7b-caa8cd8070ea.png">

The mod menu uses Dear ImGui to render the gui. The cool thing about that is that Dear ImGui has lots of possibilities.
So I will be trying make it possible to turn on/off and edit mods using this gui. And maybe add other modding tools.

Dear ImGui: https://github.com/ocornut/imgui

# Linux / Wine / Proton (Steam & Lutris) Details for the fix.

On Linux, `TextShaping.dll` is usually not loaded by Wine, so the original proxy never starts. Use the `version.dll` proxy instead.

## Install the Linux proxy

Requires only `bash` (no Python). Place the release `version.dll` next to `install.sh`, then:

```bash
./install.sh --game-dir "/path/to/Anno 1800/Bin/Win64"
```

Examples:

```bash
# Steam
./install.sh --game-dir "$HOME/.steam/steam/steamapps/common/Anno 1800/Bin/Win64"

# Lutris / Ubisoft (adjust to your path)
./install.sh --game-dir "$HOME/disks/Games/Lutris/Games/Anno 1800/Bin/Win64"
```

If Proton is not found automatically:

```bash
./install.sh \
  --game-dir "/path/to/Anno 1800/Bin/Win64" \
  --proton "$HOME/.steam/steam/steamapps/common/Proton Hotfix"
```

Developers can also use `Build/version.dll` after `bash ProxyDLL/build.sh`.

The script copies:

- `version.dll` (proxy) into `Bin/Win64`
- `version_orig.dll` from the Proton/Wine `version.dll` installation on **your** system

It does **not** download or replace the Mod Menu DX11/DX12 or Proton/Wine DLLs.

## Enable the override

Set:

- **Steam:** game launch options 
```text
WINEDLLOVERRIDES="version=n,b" %command%
```

- **Lutris:** DLL-Overrides on the Ubisoft Connect / Proton runner that starts Anno 1800

```text
Key         Value
version     n,b
```

Then start the game and press **F7**.

## Verify

With the game running:

```bash
./dll_status.sh
```

You should see `…/Bin/Win64/version.dll` (not only Proton's builtin path) and ideally `Anno1800ModMenuDX11.dll` or `DX12`.
