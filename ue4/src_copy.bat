echo off

robocopy %~dp0Source "C:\Users\owner\MyData\Dropbox\Projects\NaNRPG\Source" /mir
robocopy %~dp0Content "C:\Users\owner\MyData\DropBox\Projects\NaNRPG\Content" /mir

pause
