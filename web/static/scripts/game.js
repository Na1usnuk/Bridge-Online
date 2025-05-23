function ShowError(str) {
    alert(str)
}

function CriticalError(str) {
    console.log("CriticalError Function");
    ShowError(str);
    window.location.replace("/");
}

let IS_MY_TURN = false;
const csrf_token = document.getElementsByName("csrfmiddlewaretoken")[0].value;

CheckChanges();
window.setInterval(CheckChanges, 1000);


function ResolveRawCard(card_data) {
    let num = card_data.substring(0, card_data.length - 1)
    let suit = card_data.substring(card_data.length - 1)
    switch (suit) {
        case 's':
            {
                suit = "spades";
                break;
            }
        case 'd':
            {
                suit = "diams";
                break;
            }
        case 'c':
            {
                suit = "clubs";
                break;
            }
        case 'h':
            {
                suit = "hearts";
                break;
            }
    }
    return { num, suit }
}

function CreateCard(num, suit) {
    let card = document.createElement("button");
    card.classList.add("rectangle");
    card.setAttribute("style", "font-size: 3rem; cursor: cell; font-family: inherit;");
    if (suit == "hearts" || suit == "diams") {
        card.style.color = "red";
    }
    card.innerHTML = num + '&' + suit + ';';
    card.name = String(num) + suit;
    card.onclick = function (x) { CardClick(x); };
    return card;
}

function AddPlayerCard(num, suit) {
    document.getElementById("player_cards").append(CreateCard(num, suit));
}

function SetHeapCard(num, suit) {
    let elem = document.getElementById("heap_card");
    elem.innerHTML = num + '&' + suit + ';';
    elem.style.color = (suit == "hearts" || suit == "diams" ? "red" : "black");
}

function HeapClick() {
    if (!IS_MY_TURN) {
        ShowError("Not Your turn!")
    }
    else {
       
        fetch(window.location, {
            method: "POST",
            headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': csrf_token,
                'Action': 'PlayerEndTurn',
            }
        })
            .then(res => res.json())
            .then(json => {
                if (json['error']) {
                    ShowError(json['error'])
                    return;
                }
                SynchronizeField(json);
            });
    }
}

function DeckClick() {

    if (!GAME_STARTED) {
        StartGame();
    }
    else {
        fetch(window.location, {
            method: "POST",
            headers: {
                'Content-Type': 'application/json',
                'X-CSRFToken': csrf_token,
                'Action': 'PlayerPickCard',
            }
        })
            .then(res => res.json())
            .then(json => {
                if (json['error']) {
                    ShowError(json['error'])
                    return;
                }
                SynchronizeField(json);
            });
    }
    
}


function CardClick(card) {
    if (!IS_MY_TURN) {
        ShowError("Not Your turn!")
        return;
    }
    console.log("Card Clicked: " + card.target.name)

    fetch(window.location, {
        method: "POST",
        headers: {
            'Content-Type': 'application/json',
            'X-CSRFToken': csrf_token,
            'Action': 'PlayerTurn',
            'Card': card.target.name
        }
    })
        .then(res => res.json())
        .then(json => {
            if (json['error']) {
                ShowError(json['error'])
                return;
            }
            SynchronizeField(json);
        });
}


function SynchronizeField(data) {
    IS_MY_TURN = data['isYourTurn']
    let top = ResolveRawCard(data['topCard'])
    SetHeapCard(top.num, top.suit)
    document.getElementById("player_cards").replaceChildren()
    for (let i = 0; i < data['yourCards'].length; ++i) {
        c = ResolveRawCard(data['yourCards'][i])
        AddPlayerCard(c.num, c.suit)
    }
}


function SynchonizePlayers(data) {
    GAME_STARTED = data['isGameStarted'] == 'true' ? true : false;
    rating = document.getElementById("rating");
    rating.replaceChildren()
    for (let i = 0; i < data['players'].length; ++i) {
        p = document.createElement("p");
        p.innerHTML = data['players'][i][0] + "&nbsp;&nbsp;&nbsp;&nbsp;" + data['players'][i][1];
        rating.append(p)
    }
    if (data['top'] != undefined)
        {document.getElementById("top").innerHTML = data['top'];}
    if (data['left'] != undefined)
        {document.getElementById("left").innerHTML = data['left'];}
    if (data['right'] != undefined)
        {document.getElementById("right").innerHTML = data['right'];}
}


function CheckChanges() {
	if (!GAME_STARTED)
		PreGameCheck();
	else
        	GameCheck();
}

function GameCheck() {
    fetch(window.location, {
        method: "POST",
        headers: {
            'Content-Type': 'application/json',
            'X-CSRFToken': csrf_token,
            'Action': 'GameCheck'
        }
    })
        .then(res => res.json())
        .then(json => {
            if (json['error']) {
                CriticalError(json['error'])
            }
            SynchonizePlayers(json);
            SynchronizeField(json);
        });
}

function PreGameCheck(){
    fetch(window.location, {
        method: "POST",
        headers: {
            'Content-Type': 'application/json',
            'X-CSRFToken': csrf_token,
            'Action': 'PreGameCheck'
        }
    })
        .then(res => res.json())
        .then(json => {
            if (json['error']) {
                ShowError(json['error'])
            }
                SynchonizePlayers(json);
        });
}

function StartGame() {
    fetch(window.location, {
        method: "POST",
        headers: {
            'Content-Type': 'application/json',
            'X-CSRFToken': csrf_token,
            'Action': 'StartGame'
        }
   }).then(res => res.json())
       .then(json => {
           if (json['error']) {
               ShowError(json['error'])
               return;
           }
           GAME_STARTED = true;
           SynchronizeField(json);
    })
}

// При викиданні валета гравець повинен вибрати масть. Підказка для вибору масті
function showSuitPopup(element) {
  const popup = document.getElementById("suitPopup");
  popup.style.display = "block";

  // Розміщуємо попап біля елемента, який викликав його
  const rect = element.getBoundingClientRect();
  popup.style.left = rect.left + window.scrollX + "px";
  popup.style.top = rect.bottom + window.scrollY + 5 + "px";
}

function hideSuitPopup() {
  const popup = document.getElementById("suitPopup");
  popup.style.display = "none";
}

function selectSuit(suit) {
  alert("Вибрано масть: " + suit);
  hideSuitPopup();
  // Тут можна додати логіку обробки вибраної масті
}

// Для історії ходів
function toggleHistory(buttonSide) {
  const historyDiv = document.querySelector(".game-history");
  const showHistoryLeftButton = document.getElementById("showHistoryLeft");

  if (historyDiv.style.display === "none") {
    historyDiv.style.display = "block";
    if (buttonSide === "left") {
      showHistoryLeftButton.classList.add("hidden");
      const hideButton = document.createElement("button");
      hideButton.textContent = "Приховати історію";
      hideButton.className = "history-button left";
      hideButton.onclick = function () {
        toggleHistory("left");
      };
      showHistoryLeftButton.parentNode.insertBefore(
        hideButton,
        showHistoryLeftButton.nextSibling
      );
    }
  } else {
    historyDiv.style.display = "none";
    if (buttonSide === "left") {
      const hideButton = showHistoryLeftButton.nextSibling;
      if (hideButton && hideButton.textContent === "Приховати історію") {
        hideButton.remove();
        showHistoryLeftButton.classList.remove("hidden");
      }
    }
  }
}
