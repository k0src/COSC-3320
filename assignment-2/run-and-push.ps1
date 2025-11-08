& .\question-6.exe
git add .
$commitMessage = "Add question-6 results - $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')"
git commit -m $commitMessage
git push -u origin main