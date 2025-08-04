# Project Legacy

### 개요

Standalone, Dedicated Server 에서 동작 하며, 각종 게임에 필요한 기능들을 구현 하여, 게임 개발시 참고 가능한 레퍼런스 프로젝트.

- 개발환경
    - 엔진 : UE 5.5.4
    - IDE : Rider, VisualStudio 2022
    - VCS : Perforce, Git
    - CI/CD : Jenkins
        - Deploy 단계에서 구글드라이브로 빌드된 클라이언트와 서버를 압축하여 GoogleDrive 로 업로드
        - 구글드라이브 API 설정 후 OAuth json 파일 다운로드 및 필요한 python 라이브러리 설치
        ```text
        pip install --upgrade google-api-python-client google-auth-httplib2 google-auth-oauthlib
        ```

### 프로젝트 관련 링크

- GitHub : https://github.com/mmxbong9/Legacy
- Youtube
    - https://youtu.be/Ol0eov2wUMY?si=ZQydkZnI2KKADtqD
    - https://youtu.be/VvX_fBQ_pjo?si=KQ4H6A6-joSO9Y8o

### 기능

- Advanced Locomotion System
- Advanced Vehicle System
- Parkour System (Climbing, Mantling, Vaulting)
- Advanced Frontend UI System (CommonUI)
- Dedicated Server 빌드 및 멀티플레이

### 예정

- 근접 무기 및 스킬 시스템 + Game Ability System
- AI 전투
    - 보스
    - 몬스터
    - 아군

### 소스 엔진 패치 노트 (5.5.4)

- AVS 에서 사용하는 오디오 컨트롤러 사용시 어설션 발생 으로 ( https://github.com/EpicGames/UnrealEngine/commit/0aae2fc6714b5ccd39f4683d0b177d932463f686 ) 적용