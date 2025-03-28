전체 틀

BaseBallLib

- EGameResult : 게임 결과 상태 
- EPlayerTurn : 현재 턴 상태
- FPlayerNumberList : 
- RandomNumbers() : 랜덤 숫자 생성
- Refree() : Strike, Ball 체크
- IsGameOver : 게임 종료 여부

BaseBallGameMode

- PostLogin() : PlayerController에 PlayerIndex 할당
- ProcessChatMessage() :  플레이어의 채팅 받음 -> 입력 형식, 턴 검증 수행 ->  Refree 호출 후 Strike, Ball 계산 -> GameState의 Mulit_UpdateMessage()를 불러와 결과 메세지를 모든 클라이언트에 전파

BaseBallGameState

- BeginPlay() : 게임 시작 시 목표 숫자를 생성, 플레이어 수에 맞춰 PlayerAttempts, CurrentStrikes 배열 초기화
- Mulit_UpdateMessage() : NetMultiCast RPC로 모든 클라이언트에 결과 메세지를 전달.

BaseBallPlayerController()

- Beginplay() : 로컬 컨트롤러일시 ChatWidget을 AddViewport
- SendMessage() : 플레이어의 채팅 메세지를 받음
  -> 서버 권한(Authority)일 경우 GameMode()의 ProcessChatMessage() 호출
  -> 클라이언트일 경우 Server_SendMessage RPC를 호출해 메세지를 서버로 전송
- 오류 발생 시 Client_CheckValidError()로 오류 메시지 표시

ChatWidget

- NativeConstruct() : 기본 초기화 작업 수행 
- OnTextCommitted() : 사용자가 채팅을 입력 후 Enter시 PlayerController에 전달 -> AddChatMessage()를 통해 결과 메세지 표시
