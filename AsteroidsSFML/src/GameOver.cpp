#include "pch.h"
#include "GameOver.h"

GameOver::GameOver(Game* game, const std::string& textString, const int fontSize, const float x, const float y) :
    GameText(game, textString, fontSize, x, y),
    name(game->getOverGui().get<tgui::EditBox>("playername")),
    submit(game->getOverGui().get<tgui::Button>("submit")) {
}

void GameOver::enter() {
    name->setText({ "" }); // clear out the player name
    loadHighScores();
    
    if (game->getScore() > lowestHighScore) {
        name->setVisible(true);
        name->setFocused(true);
        submit->setVisible(true);
    } else {
        name->setVisible(false);
        submit->setVisible(false);
    }
}
void GameOver::leave() {
    game->resetPlay();
}

void GameOver::loadHighScores() {
    lowestHighScore = 1;
    sf::Http http("host.domain.com");
    sf::Http::Request request("scores/scores.aspx?apikey=11&action=read");
    sf::Http::Response response = http.sendRequest(request, sf::milliseconds(500));
    if (response.getStatus() == sf::Http::Response::Ok) {
        displayHighScores(response.getBody());
    } else {
        std::cout << "Warning: unable to load high scores\n";
    }
}

void GameOver::displayHighScores(const std::string& data) {
    unsigned int prev = 0, pos = 0, len = data.length();
    std::stringstream text;
    text << "     - GAME OVER -\n\n";
    while (pos < len && prev < len) {
        pos = data.find(';', prev);
        if (pos == std::string::npos) pos = len;
        std::string token = data.substr(prev, pos - prev);
        if (!token.empty()) {
            unsigned int pos = token.find(",");
            text << std::setw(10) << token.substr(0, pos) << ' '
                << std::setw(6) << token.substr(pos + 1) << '\n';
            lowestHighScore = atoi(token.substr(pos + 1).c_str());
        }
        prev = pos + 1;
    }
    setText(text.str());
}

// this static method is called when the user clicks on the submit button
void GameOver::saveScore(Game* game) {
    static const tgui::EditBox::Ptr name = game->getOverGui().get<tgui::EditBox>("playername");

    std::stringstream url;
    url << "scores/scores.aspx?apikey=11&action=write"
        << "&name=" << name->getText().toAnsiString()
        << "&score=" << game->getScore();
    sf::Http http("host.domain.com");
    sf::Http::Request request(url.str());
    sf::Http::Response response = http.sendRequest(request, sf::milliseconds(500));
    if (response.getStatus() != sf::Http::Response::Ok) {
        std::cout << "Warning: unable to save score" << std::endl;
    }
}
