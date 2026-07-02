#include "ASTutorialGameMode.h"
#include "ASTutorialCharacter.h"
#include "GameFramework/PlayerController.h"

AASTutorialGameMode::AASTutorialGameMode()
{
    DefaultPawnClass = AASTutorialCharacter::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
}