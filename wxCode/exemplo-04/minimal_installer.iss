; Instalador Inno Setup para Minimal Application
; Salve como: minimal_installer.iss

#define MyAppName "Minimal"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Seu Nome"
#define MyAppURL "https://www.seusite.com"
#define MyAppExeName "minimal.exe"
#define MyAppDescription "Minimal wxWidgets Application"

[Setup]
; ==========================================
; INFORMAÇÕES BÁSICAS
; ==========================================
AppId={{A1B2C3D4-5678-90AB-CDEF-1234567890AB}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppComments={#MyAppDescription}

; Diretório de instalação
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}

; ==========================================
; CUSTOMIZAÇÃO VISUAL
; ==========================================
; Descomente se você tiver as imagens
;SetupIconFile=minimal.ico
;WizardImageFile=wizard-image.bmp
;WizardSmallImageFile=wizard-small-image.bmp

WizardStyle=modern
DisableWelcomePage=no

; Mensagens personalizadas
WelcomeLabel1=Bem-vindo ao instalador do {#MyAppName}!
WelcomeLabel2=Este programa irá instalar {#MyAppName} {#MyAppVersion} no seu computador.%n%nÉ recomendado fechar todos os outros programas antes de continuar.
FinishedLabel=A instalação foi concluída com sucesso!%n%nVocê pode executar {#MyAppName} através dos atalhos criados.

; ==========================================
; CONFIGURAÇÕES DE SAÍDA
; ==========================================
OutputDir=installer_output
OutputBaseFilename=minimal-setup-{#MyAppVersion}-x64
Compression=lzma2/ultra64
SolidCompression=yes

; ==========================================
; PRIVILÉGIOS E ARQUITETURA
; ==========================================
PrivilegesRequired=admin
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
UninstallDisplayIcon={app}\{#MyAppExeName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"

[CustomMessages]
english.AppRunning={#MyAppName} is currently running. Please close it before continuing.
english.InstallSuccess={#MyAppName} has been successfully installed!
english.UninstallQuestion=Are you sure you want to uninstall {#MyAppName}?

brazilianportuguese.AppRunning={#MyAppName} está em execução. Por favor, feche-o antes de continuar.
brazilianportuguese.InstallSuccess={#MyAppName} foi instalado com sucesso!
brazilianportuguese.UninstallQuestion=Tem certeza que deseja desinstalar {#MyAppName}?

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked; OnlyBelowVersion: 6.1

[Files]
; ==========================================
; ARQUIVOS DA PASTA build/gccmsw
; ==========================================
; Executável principal
Source: "build\gccmsw\minimal.exe"; DestDir: "{app}"; Flags: ignoreversion

; DLLs necessárias do MinGW
Source: "build\gccmsw\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "build\gccmsw\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion

; Makefile (opcional - para referência)
Source: "build\Makefile"; DestDir: "{app}\build"; Flags: ignoreversion

; Arquivos adicionais (descomente se existirem)
;Source: "README.txt"; DestDir: "{app}"; Flags: ignoreversion isreadme
;Source: "LICENSE.txt"; DestDir: "{app}"; Flags: ignoreversion
;Source: "minimal.ico"; DestDir: "{app}"; Flags: ignoreversion

; NOTA: Se você tiver outros arquivos necessários (recursos, configs, etc),
; adicione-os aqui:
;Source: "resources\*"; DestDir: "{app}\resources"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; Atalho no Menu Iniciar
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Comment: "Executar {#MyAppName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

; Atalho na Área de Trabalho
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; Comment: "{#MyAppName} {#MyAppVersion}"

; Atalho na Barra de Inicialização Rápida (Windows 7 e anterior)
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: quicklaunchicon

[Run]
; Executar o aplicativo após a instalação
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; Remover arquivos criados durante o uso do aplicativo (se houver)
Type: filesandordirs; Name: "{app}\logs"
Type: filesandordirs; Name: "{app}\cache"
Type: filesandordirs; Name: "{app}\temp"

[Code]
var
  ProgressPage: TOutputProgressWizardPage;

// ==========================================
// VERIFICAR SE APLICATIVO ESTÁ RODANDO
// ==========================================
function InitializeSetup(): Boolean;
var
  ResultCode: Integer;
  ProcessList: string;
begin
  Result := True;
  
  // Verificar se minimal.exe está rodando
  if Exec('tasklist.exe', '/FI "IMAGENAME eq minimal.exe" /NH', '', SW_HIDE, 
          ewWaitUntilTerminated, ResultCode) then
  begin
    // Se o processo existe, pedir para fechar
    if ResultCode = 0 then
    begin
      if MsgBox(ExpandConstant('{cm:AppRunning}'), mbConfirmation, MB_YESNO) = IDYES then
      begin
        // Tentar fechar o processo
        Exec('taskkill.exe', '/F /IM minimal.exe', '', SW_HIDE, 
             ewWaitUntilTerminated, ResultCode);
        Sleep(1000);
      end
      else
      begin
        Result := False;
      end;
    end;
  end;
end;

// ==========================================
// BARRA DE PROGRESSO PERSONALIZADA
// ==========================================
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then
  begin
    ProgressPage := CreateOutputProgressPage('Instalando {#MyAppName}', 
                                             'Por favor aguarde...');
    ProgressPage.Show;
    try
      ProgressPage.SetProgress(0, 100);
      ProgressPage.SetText('Copiando minimal.exe...', '');
      Sleep(300);
      
      ProgressPage.SetProgress(30, 100);
      ProgressPage.SetText('Copiando DLLs necessárias...', '');
      Sleep(300);
      
      ProgressPage.SetProgress(60, 100);
      ProgressPage.SetText('Criando atalhos...', '');
      Sleep(300);
      
      ProgressPage.SetProgress(90, 100);
      ProgressPage.SetText('Registrando aplicativo...', '');
      Sleep(300);
      
      ProgressPage.SetProgress(100, 100);
      ProgressPage.SetText('Finalizando instalação...', '');
      Sleep(300);
    finally
      ProgressPage.Hide;
    end;
  end;
  
  if CurStep = ssPostInstall then
  begin
    // Mensagem de sucesso
    MsgBox(ExpandConstant('{cm:InstallSuccess}') + #13#10#13#10 +
           'Arquivos instalados em:' + #13#10 +
           ExpandConstant('{app}') + #13#10#13#10 +
           'Tamanho total: aproximadamente 5 MB', 
           mbInformation, MB_OK);
  end;
end;

// ==========================================
// VALIDAR REQUISITOS DO SISTEMA
// ==========================================
function NextButtonClick(CurPageID: Integer): Boolean;
var
  Version: TWindowsVersion;
begin
  Result := True;
  
  if CurPageID = wpWelcome then
  begin
    GetWindowsVersionEx(Version);
    
    // Verificar se é Windows 7 ou superior
    if (Version.Major < 6) or ((Version.Major = 6) and (Version.Minor < 1)) then
    begin
      MsgBox('Este aplicativo requer Windows 7 ou superior.' + #13#10 +
             'Seu sistema: Windows ' + IntToStr(Version.Major) + '.' + IntToStr(Version.Minor),
             mbError, MB_OK);
      Result := False;
    end;
  end;
  
  if CurPageID = wpSelectDir then
  begin
    // Verificar espaço em disco (mínimo 50 MB)
    if GetSpaceOnDisk(ExpandConstant('{app}'), False, 0, 0, 0) < (50 * 1024 * 1024) then
    begin
      MsgBox('Espaço insuficiente em disco!' + #13#10 +
             'São necessários pelo menos 50 MB livres.', 
             mbError, MB_OK);
      Result := False;
    end;
  end;
end;

// ==========================================
// DESINSTALAÇÃO
// ==========================================
procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  Response: Integer;
  ResultCode: Integer;
begin
  if CurUninstallStep = usUninstall then
  begin
    // Fechar o aplicativo se estiver rodando
    Exec('taskkill.exe', '/F /IM minimal.exe', '', SW_HIDE, 
         ewWaitUntilTerminated, ResultCode);
    Sleep(500);
  end;
  
  if CurUninstallStep = usPostUninstall then
  begin
    // Perguntar se deseja remover configurações
    Response := MsgBox('Deseja remover também os arquivos de configuração?' + #13#10 +
                       '(Dados salvos pelo aplicativo)', 
                       mbConfirmation, MB_YESNO);
    if Response = IDYES then
    begin
      DelTree(ExpandConstant('{userappdata}\{#MyAppName}'), True, True, True);
      DelTree(ExpandConstant('{localappdata}\{#MyAppName}'), True, True, True);
    end;
    
    MsgBox('{#MyAppName} foi desinstalado com sucesso!', mbInformation, MB_OK);
  end;
end;

// ==========================================
// INFORMAÇÕES NA TELA "PRONTO PARA INSTALAR"
// ==========================================
function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo, 
                         MemoTypeInfo, MemoComponentsInfo, MemoGroupInfo, 
                         MemoTasksInfo: String): String;
begin
  Result := '';
  
  if MemoDirInfo <> '' then
    Result := Result + MemoDirInfo + NewLine + NewLine;
  
  if MemoGroupInfo <> '' then
    Result := Result + MemoGroupInfo + NewLine + NewLine;
    
  if MemoTasksInfo <> '' then
    Result := Result + MemoTasksInfo + NewLine + NewLine;
  
  // Adicionar informações personalizadas
  Result := Result + 'Arquivos a serem instalados:' + NewLine;
  Result := Result + Space + '- minimal.exe' + NewLine;
  Result := Result + Space + '- libgcc_s_seh-1.dll' + NewLine;
  Result := Result + Space + '- libstdc++-6.dll' + NewLine;
  Result := Result + NewLine;
  Result := Result + 'Espaço necessário: ~5 MB';
end;

