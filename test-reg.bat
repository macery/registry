@echo off
Echo _______________________________________
cd bin
echo Check for DWord that is not present
get-dword Software\HPNC\SneakerTest delete
echo Check for DWord present
echo Expect a numerical value:
get-dword Software\HPNC\SneakerTest Exists
echo Set DWord not present
echo expect Value Created:
set-dword Software\HPNC\SneakerTest newDW 1
echo Set DWord notepadpresent
echo expect Value Updated
set-dword Software\HPNC\SneakerTest Exists 0
echo Set DWord present same value
set-dword Software\HPNC\SneakerTest Exists 0
echo expect No Change needed
echo reset DWord 
echo expect Value Updated
set-dword Software\HPNC\SneakerTest Exists 1
Echo _______________________________________
@echo
echo Read a string that doesn't Exists
echo Expect not found
get-string Software\HPNC\SneakerTest ImNotHere
echo Read existing string value
get-string Software\HPNC\SneakerTest Owner
echo Set new string value
set-string Software\HPNC\SneakerTest newString DeleteMe
echo update string value
set-string Software\HPNC\SneakerTest update change
echo try to set the value to itself
set-string Software\HPNC\SneakerTest update change
echo resetting that string value
set-string Software\HPNC\SneakerTest update start
Echo _______________________________________
cd ..