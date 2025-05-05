// Код для обробки події click кнопки "Профіль"
document.getElementById('profile-button').addEventListener('click', function() {
  window.location.href = 'profile.html';
});

// Код для обробки події click кнопки "Назад" на кожній сторінці
document.querySelectorAll('#back-button').forEach(button => {
  button.addEventListener('click', function() {
      window.location.href = 'index.html';
  });
});

// Код для обробки події click кнопки "Почати гру" на сторінці guest.html
const playGuestButton = document.getElementById('play-guest-button');
if (playGuestButton) {
  playGuestButton.addEventListener('click', function() {
      window.location.href = 'game.html';
  });
}

// Код для обробки подій на сторінці shop.html
const buyButtons = document.querySelectorAll('.shop__item-buy');
buyButtons.forEach(button => {
  button.addEventListener('click', () => {
      alert('Товар додано до кошика!');
  });
});
