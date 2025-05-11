
const cards = document.querySelectorAll(".card");
cards.forEach(addCardElements);

function addCardElements(card) {
  const value = card.dataset.value;
  // const suit = card.dataset.suit

  const valueAsNumber = parseInt(value);
  if (isNaN(valueAsNumber)) {
    card.append(createPip());
  } else {
    for (let i = 0; i < valueAsNumber; i++) {
      card.append(createPip());
    }
  }

  card.append(createCornerNumber("top", value));
  card.append(createCornerNumber("bottom", value));
}

function createCornerNumber(position, value) {
  const corner = document.createElement("div");
  corner.textContent = value;
  corner.classList.add("corner-number");
  corner.classList.add(position);
  return corner;
}

function createPip() {
  const pip = document.createElement("div");
  pip.classList.add("pip");
  return pip;
}

function addNewCard(suit, value) {
  const newCard = document.createElement("div");
  newCard.classList.add("card");
  newCard.dataset.suit = suit;
  newCard.dataset.value = value;

  const playerCardsContainer = document.getElementById("player_cards"); // Припускаємо, що у вас є цей контейнер
  if (playerCardsContainer) {
    playerCardsContainer.appendChild(newCard);
    addCardElements(newCard); // Застосовуємо логіку до новоствореної картки
  } else {
    console.error("Контейнер з id 'player_cards' не знайдено.");
  }
}

function AddPlayerCard(num, suit) {
    document.getElementById("player_cards").append(addNewCard(suit, num));
}

// Приклади додавання нових карток після завантаження DOM
document.addEventListener('DOMContentLoaded', () => {
  addNewCard("heart", "Q");
});
