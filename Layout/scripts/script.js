const buttons = document.querySelectorAll("main > div#switch > button");
const forms = document.querySelectorAll("main form");

buttons[0].addEventListener("click", (event) => {
    buttons[0].classList.remove("disabled");
    buttons[1].classList.add("disabled");
    forms[0].parentElement.style.display = "flex";
    forms[1].style.display = "none";
})
buttons[1].addEventListener("click", (event) => {
    buttons[1].classList.remove("disabled");
    buttons[0].classList.add("disabled");
    forms[0].parentElement.style.display = "none";
    forms[1].style.display = "flex";
});