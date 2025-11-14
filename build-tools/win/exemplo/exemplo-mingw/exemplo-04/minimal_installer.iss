; Instalador Inno Setup para Minimal Application
; Salve como: minimal_installer.iss

#define MyAppName "Minimal"
#define MyAppVersion "1.0.0"
#define MyAppPublisher "Ivan Lopes"
#define MyAppURL "https://www.seusite.com"
#define MyAppExeName "run_minimal.bat"
#define MyAppDescription "Minimal wxWidgets Application"

[Setup]
; ==========================================
; INFORMAÇÕES BÁSICAS
; ==========================================
AppId={{A1B2C3D4-5678-90AB-CDEF-1234567890AB}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppComments={#MyAppDescription}

; Diretório de instalação (Program Files) — requer admin
DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}

; ==========================================
; CUSTOMIZAÇÃO VISUAL
; ==========================================
;SetupIconFile=minimal.ico
;WizardImageFile=wizard-image.bmp
;WizardSmallImageFile=wizard-small-image.bmp

WizardStyle=modern
DisableWelcomePage=no
DisableFinishedPage=no

; ==========================================
; CONFIGURAÇÕES DE SAÍDA
; ==========================================
OutputDir=installer_output
OutputBaseFilename=minimal-setup-{#MyAppVersion}-x64
Compression=lzma2/normal
SolidCompression=no

; ==========================================
; PRIVILÉGIOS E ARQUITETURA
; ==========================================
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
UninstallDisplayIcon={app}\{#MyAppExeName}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "brazilianportuguese"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"

[CustomMessages]
english.AppRunning={#MyAppName} is currently running. Please close it before continuing.
english.InstallSuccess={#MyAppName} has been successfully installed!
english.UninstallQuestion=Are you sure you want to uninstall {#MyAppName}?
english.WelcomeMessage=Welcome to {#MyAppName} Setup Wizard!%n%nThis will install {#MyAppName} {#MyAppVersion} on your computer.
english.FinishMessage=Installation completed successfully!%n%nYou can run {#MyAppName} using the created shortcuts.

brazilianportuguese.AppRunning={#MyAppName} está em execução. Por favor, feche-o antes de continuar.
brazilianportuguese.InstallSuccess={#MyAppName} foi instalado com sucesso!
brazilianportuguese.UninstallQuestion=Tem certeza que deseja desinstalar {#MyAppName}?
brazilianportuguese.WelcomeMessage=Bem-vindo ao Assistente de Instalação do {#MyAppName}!%n%nEste programa irá instalar {#MyAppName} {#MyAppVersion} no seu computador.
brazilianportuguese.FinishMessage=Instalação concluída com sucesso!%n%nVocê pode executar {#MyAppName} através dos atalhos criados.

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce

[Files]
	; ==========================================
	; ARQUIVOS DA PASTA build/gccmsw
; ==========================================
; Executável principal
Source: "build-mingw64\minimal.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "build-mingw64\run_minimal.bat"; DestDir: "{app}"; Flags: ignoreversion

; Ex.: recursos adicionais
;Source: "resources\*"; DestDir: "{app}\resources"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
; Atalho no Menu Iniciar (todos os usuários)
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Comment: "Executar {#MyAppName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

; Atalho na Área de Trabalho (todos os usuários)
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon; Comment: "{#MyAppName} {#MyAppVersion}"

[Run]
; Executar o aplicativo após a instalação
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; Remover diretórios de dados do aplicativo (área comum, pois é instalação admin)
Type: filesandordirs; Name: "{commonappdata}\{#MyAppName}"
Type: filesandordirs; Name: "{app}\logs"
Type: filesandordirs; Name: "{app}\cache}"
Type: filesandordirs; Name: "{app}\temp"

[Code]
var
  ProgressPage: TOutputProgressWizardPage;

procedure InitializeWizard();
begin
  { Mensagens de boas-vindas vêm dos .isl ou CustomMessages }
end;

function InitializeSetup(): Boolean;
var
  ResultCode: Integer;
begin
  Result := True;
  { Tentar fechar o app se estiver em execução }
  Exec('taskkill.exe', '/F /IM minimal.exe', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
end;

procedure CurStepChanged(CurStep: TSetupStep);
begin
  if CurStep = ssInstall then
  begin
    ProgressPage := CreateOutputProgressPage('Instalando {#MyAppName}', 'Por favor aguarde...');
    ProgressPage.Show;
    try
      ProgressPage.SetProgress(0, 100);
      ProgressPage.SetText('Copiando minimal.exe...', '');
      Sleep(200);

      ProgressPage.SetProgress(30, 100);
      ProgressPage.SetText('Copiando DLLs necessárias...', '');
      Sleep(200);

      ProgressPage.SetProgress(60, 100);
      ProgressPage.SetText('Criando atalhos...', '');
      Sleep(200);

      ProgressPage.SetProgress(90, 100);
      ProgressPage.SetText('Registrando aplicativo...', '');
      Sleep(200);

      ProgressPage.SetProgress(100, 100);
      ProgressPage.SetText('Finalizando instalação...', '');
      Sleep(150);
    finally
      ProgressPage.Hide;
    end;
  end;

  if CurStep = ssPostInstall then
  begin
    MsgBox(ExpandConstant('{cm:InstallSuccess}') + #13#10#13#10 +
           'Arquivos instalados em:' + #13#10 +
           ExpandConstant('{app}'),
           mbInformation, MB_OK);
  end;
end;

// ====== Checagem de espaço em disco (fix 64-bit) ======
// Mantém a sua função NextButtonClick, mas usa GetSpaceOnDisk64 para evitar overflow.
function NextButtonClick(CurPageID: Integer): Boolean;
var
  Version: TWindowsVersion;
  // valores em bytes, 64-bit
  FreeB, TotalB: Int64;
  // valor em MB apenas para exibição
  FreeMB: Int64;
  HaveSpace: Boolean;
begin
  Result := True;

  if CurPageID = wpWelcome then
  begin
    GetWindowsVersionEx(Version);
    // Requer Windows 7 (6.1) ou superior
    if (Version.Major < 6) or ((Version.Major = 6) and (Version.Minor < 1)) then
    begin
      MsgBox('Este aplicativo requer Windows 7 ou superior.' + #13#10 +
             'Seu sistema: Windows ' + IntToStr(Version.Major) + '.' + IntToStr(Version.Minor),
             mbError, MB_OK);
      Result := False;
      Exit;
    end;
  end;

  if CurPageID = wpSelectDir then
  begin
    // Mede com 64 bits em BYTES para não estourar em discos grandes:
    HaveSpace := GetSpaceOnDisk64(WizardDirValue, FreeB, TotalB);
    // Converte para MB apenas para mostrar ao usuário
    FreeMB := FreeB div 1048576; // 1024*1024

    // Requer no mínimo 50 MB livres
    Result := True;
//    if (not HaveSpace) or (FreeB < (50 * 1048576)) then
//    begin
//      MsgBox('Espaço insuficiente em disco!' + #13#10 +
//             'São necessários pelo menos 50 MB livres.' + #13#10 +
//             'Livre detectado: ~' + IntToStr(FreeMB) + ' MB',
//             mbError, MB_OK);
//      Result := False;
//      Exit;
//    end;
  end;
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
  ResultCode: Integer;
  Response: Integer;
begin
  if CurUninstallStep = usUninstall then
  begin
    Exec('taskkill.exe', '/F /IM run_minimal.bat', '', SW_HIDE, ewWaitUntilTerminated, ResultCode);
    Sleep(300);
  end;

  if CurUninstallStep = usPostUninstall then
  begin
    Response := MsgBox('Deseja remover também os arquivos de configuração (dados compartilhados)?',
                       mbConfirmation, MB_YESNO);
    if Response = IDYES then
    begin
      DelTree(ExpandConstant('{commonappdata}\{#MyAppName}'), True, True, True);
    end;

    MsgBox('{#MyAppName} foi desinstalado com sucesso!', mbInformation, MB_OK);
  end;
end;

function UpdateReadyMemo(Space, NewLine, MemoUserInfoInfo, MemoDirInfo,
  MemoTypeInfo, MemoComponentsInfo, MemoGroupInfo, MemoTasksInfo: String): String;
begin
  Result := '';

  if MemoDirInfo <> '' then
    Result := Result + MemoDirInfo + NewLine + NewLine;

  if MemoGroupInfo <> '' then
    Result := Result + MemoGroupInfo + NewLine + NewLine;

  if MemoTasksInfo <> '' then
    Result := Result + MemoTasksInfo + NewLine + NewLine;

  Result := Result + 'Arquivos a serem instalados:' + NewLine;
  Result := Result + Space + '- minimal.exe' + NewLine;
  Result := Result + Space + '- run_minimal.bat' + NewLine;
  Result := Result + NewLine + 'Espaço necessário: ~5 MB';
end;
