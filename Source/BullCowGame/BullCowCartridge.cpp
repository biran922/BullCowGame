// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetUpGame(); //setting up game
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetUpGame();
    }
    else {
        ProcessGuess(Input);
    }
}


void UBullCowCartridge::SetUpGame() {
    //weclome the player
    PrintLine(TEXT("Welcome to Bull Cows\n"));
    valid = GetValidWords(Words);
    HiddenWord = valid[FMath::RandRange(0, valid.Num() - 1)];
    Lives = HiddenWord.Len();
    PrintLine(TEXT("You have %i Lives"), Lives);
   // PrintLine((TEXT("The HiddenWord is :%s")), *HiddenWord); //debug line
    bGameOver = false;
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("Type in you'r guess. \npress enter to continue...."));  //prompt player for guess
   
}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
    PrintLine(TEXT("press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    if (HiddenWord == Guess)
    {
        PrintLine(TEXT("You Won!!"));
        EndGame();
        return;
    }
    else
    {
        if (!Check_Isogram(Guess)) {
            PrintLine(TEXT("Guess is not an Isogram, try again!"));
            return;
        }
        if (HiddenWord.Len() != Guess.Len()) {
            PrintLine(TEXT("Wrong length, try Inputing again!\n You have %i lives remaining"), Lives);
            return;
        }
        else {
            Lives--;
            PrintLine(TEXT("Lost a life!"));
            PrintLine(TEXT("current Lives: %i"), Lives);
            if (Lives <= 0) {
                ClearScreen();
                PrintLine(TEXT("You have reached 0 lives and lost the game!!"));
                PrintLine((TEXT("The HiddenWord is :%s")), *HiddenWord);
                EndGame();
            }
        }
    }
    FBullCowCount Count = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Count.Bulls,Count.Cows);
}

bool UBullCowCartridge::Check_Isogram(const FString& Word) const
{
    for (size_t i{ 0 }; i < Word.Len() - 1; i++) {
        for (size_t j{ i+1 }; j < Word.Len(); j++) {
            if (Word[i] == Word[j])
                return false;
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &Words) const {
    TArray<FString> valid;
    for (auto w : Words) {
        if (UBullCowCartridge::Check_Isogram(w)) {
            valid.Emplace(w);
        }
    }
    return valid;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const {
    FBullCowCount c;
    for (int32 i{ 0 }; i < Guess.Len(); i++) {
        if (Guess[i] == HiddenWord[i])
        {
            c.Bulls++;
            continue;
        }
        for (int32 j{ 0 }; j < Guess.Len(); j++)
         {
            if (Guess[i] == HiddenWord[j]) {
                c.Cows++;
                break;
             }           
         }
    }
    return c;
}