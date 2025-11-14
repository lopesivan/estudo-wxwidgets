#NoEnv
#SingleInstance, Force
SendMode, Input
SetTitleMatchMode, 2
SetControlDelay, -1
SetKeyDelay, 10, 10
SetWorkingDir, %A_ScriptDir%

; ===========================================
; CONFIG
msysBat  := "C:\Git\git-bash.exe"
destWin  := ""   ; será preenchido com "ahk_pid NNN"
; ===========================================

; 1) Abre o Git Bash e captura o PID do lançador
Run, "%msysBat%",, , pidLauncher
if (ErrorLevel) {
    MsgBox, 16, Erro, Falha ao abrir:`n%msysBat%
    ExitApp
}

; 2) Descobre a janela “real” do terminal e pega o PID certo (mintty ou conhost)
termPID := WaitForGitBashWindow(pidLauncher, 10000)   ; timeout 10s
if (!termPID) {
    MsgBox, 16, Erro, Não encontrei a janela do Git Bash / mintty.
    ExitApp
}
destWin := "ahk_pid " . termPID

; 3) (opcional) dá foco
WinActivate, %destWin%
WinWaitActive, %destWin%,, 3

; 4) Envia comandos (via PID), preservando espaços com {Text}
;    Exemplo: ir para Desktop (formato /c/...), testar git e sair
;SendLineToPID(termPID, "echo 'READY'")
;SendLineToPID(termPID, "git --version")
;SendLineToPID(termPID, "git init")
;SendLineToPID(termPID, "pwd")

; Para sair (se quiser):
;SendLineToPID(termPID, "exit")

ExitApp


; ============ FUNÇÕES ============

; Espera a janela do terminal e retorna o PID “certo”
WaitForGitBashWindow(launcherPID, timeoutMs := 10000) {
    start := A_TickCount

    ; Caso raro: o próprio lançador tenha janela
    if WinExist("ahk_pid " . launcherPID)
        return launcherPID

    loop {
        if (A_TickCount - start > timeoutMs)
            return 0

        ; Preferir mintty (Git Bash padrão)
        if WinExist("ahk_exe mintty.exe") {
            ; pega a janela mais “recente” do mintty (a ativa ou a primeira encontrada)
            WinGet, pidMint, PID, ahk_exe mintty.exe
            if (pidMint)
                return pidMint
        }

        ; Fallback: algumas instalações mostram janela do próprio git-bash.exe
        if WinExist("ahk_exe git-bash.exe") {
            WinGet, pidGB, PID, ahk_exe git-bash.exe
            if (pidGB)
                return pidGB
        }

        ; Fallback adicional: console clássico
        if WinExist("ahk_class ConsoleWindowClass") {
            WinGet, pidCon, PID, ahk_class ConsoleWindowClass
            if (pidCon)
                return pidCon
        }

        Sleep, 100
    }
}

; Envia uma linha de texto (com espaços) + Enter para o PID alvo
SendLineToPID(pid, text) {
    win := "ahk_pid " . pid
    ; {Text} evita que espaços/sinais sejam interpretados como teclas
    ControlSend,, {Text}%text%, %win%
    ControlSend,, {Enter}, %win%
    Sleep, 50
}

