var GAME_STARTED = false;



function CreateCard(num, suit) {
    let card = document.createElement("button");
    card.classList.add("rectangle");
    card.setAttribute("style", "font-size: 3rem; cursor: cell; font-family: inherit;");
    if (suit == "hearts" || suit == "diams") {
        card.style.color = "red";
    }
    card.innerHTML = num + '&' + suit + ';';
    card.name = String(num) + suit;
    return card;
}

function AddPlayerCard(num, suit) {
    document.getElementById("player_cards").append(CreateCard(num, suit));
}

function RemovePlayerCard(num, suit) {
    document.getElementById("player_cards").removeChild(document.getElementsByName(num + suit)[0]);
}

function SetHeapCard(num, suit) {
    let elem = document.getElementById("heap_card");
    elem.innerHTML = num + '&' + suit + ';';
    elem.style.color = (suit == "hearts" || suit == "diams" ? "red" : "black");
}

function DeckClick() {
  
        StartGame();
        GAME_STARTED = true;
    
}



function SynchronizeField(data) {

}


function StartGame() {
    fetch("/game/", {
        method: "POST",
        headers: { 'Content-Type': 'application/json' },
        body: ({ 'StartGame': true })
    }).then(res => res.json)
      .then(json => {
          console.log("response: ", json)
    })
}
