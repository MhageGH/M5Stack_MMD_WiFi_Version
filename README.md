# M5Stack_MMD_WiFi_Version
## How to use
1. Write M5Stack_MMD_WiFi_Version.ino to M5Stack.
2. Build and execute MmdViewerSolution.sln by VisualStudio 2017.
3. Input IP to a Textbox in MMD Viewer Form and click Start button.
4. You can rotate the 3D model by moving M5Stack body like AR. This is realized by 6-axis sensor mounted in M5Stack FIRE or M5Stack GRAY.
## movie 
https://twitter.com/Tw_Mhage/status/1132506761087180800<br>
https://twitter.com/Tw_Mhage/status/1134694811074682880
## PC requirement 
- Install DirectX SDK (June 2010)
- Link project to the folder of DirectX SDK
- If you use Visual Studio other than Visual Studio 2017, you have to build bullet library with bullet-2.82-r2704 and put lib files into VisualStudio2017/BulletLib.
## known issues
- Support only PMD. Not support PMX.
- Physical operation doesn't work well at some models
