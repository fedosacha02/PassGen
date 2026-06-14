const buttons = document.querySelectorAll("main div#switch > button");
const forms = document.querySelectorAll("main form");
const error_message = document.getElementById('error-msg');
const params = new URLSearchParams(window.location.search);
const reg_error = params.get('reg_error');
const log_error = params.get('log_error');



buttons[0].addEventListener("click", (event) => {
    buttons[0].classList.remove("disabled");
    buttons[1].classList.add("disabled");
    forms[0].style.display = "flex";
    forms[1].style.display = "none";
    error_message.textContent = "";
    params.delete("reg_error");
    window.history.replaceState({}, document.title, "/login"); //if you don't want to reload
});

buttons[1].addEventListener("click", (event) => {
    buttons[1].classList.remove("disabled");
    buttons[0].classList.add("disabled");
    forms[0].style.display = "none";
    forms[1].style.display = "flex";
    error_message.textContent = "";
    params.delete("log_error");
    window.history.replaceState({}, document.title, "/login"); //if you don't want to reload
});



const reg_messages = {
    username_taken:    'That username is already taken.',
    password_mismatch: 'Passwords do not match.',
};

const log_messages = {
    invalid_credentials: 'The username or password is invalid.'
}

if (reg_error && reg_messages[reg_error]) {
    buttons[1].classList.remove("disabled");
    buttons[0].classList.add("disabled");
    forms[0].style.display = "none";
    forms[1].style.display = "flex";
    error_message.textContent = reg_messages[reg_error];
}
else if(log_error && log_messages[log_error]){
    buttons[0].classList.remove("disabled");
    buttons[1].classList.add("disabled");
    forms[1].style.display = "none";
    forms[0].style.display = "flex";
    error_message.textContent = log_messages[log_error];
}