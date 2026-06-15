const buttons = document.querySelectorAll("main div#switch > button");
const forms = document.querySelectorAll("main form");
const error_message = document.getElementById('error-msg');
const h2 = document.querySelector("header > h2");



console.log(document.cookie);

forms.forEach(
    (x) => {
        x.addEventListener("submit", () => {
            document.cookie = "reg_error1=; Max-Age=0";
            document.cookie = "reg_error2=; Max-Age=0";
            document.cookie = "log_error=; Max-Age=0";
        })
    }
);

buttons[0].addEventListener("click", (event) => {
    buttons[0].classList.remove("disabled");
    buttons[1].classList.add("disabled");
    forms[0].style.display = "flex";
    forms[1].style.display = "none";
    error_message.textContent = "";
    document.cookie = "reg1_error=; Max-Age=0";
    document.cookie = "reg2_error=; Max-Age=0";
    document.cookie = "log_error=; Max-Age=0";
    h2.style.display = "block";
});

buttons[1].addEventListener("click", (event) => {
    buttons[1].classList.remove("disabled");
    buttons[0].classList.add("disabled");
    forms[0].style.display = "none";
    forms[1].style.display = "flex";
    error_message.textContent = "";
    document.cookie = "reg1_error=; Max-Age=0";
    document.cookie = "reg2_error=; Max-Age=0";
    document.cookie = "log_error=; Max-Age=0";
    h2.style.display = "block";
});



const errors = {
    username_taken: 'That username is already taken. Please, choose another\n',
    passwords_mismatch: 'Passwords do not match. Please, check the spelling\n',
    invalid_credentials: 'The username or password is invalid.\n'
};



if(document.cookie){
    h2.style.display = "none";
    document.cookie.split("; ").map((x) => x.split('=')).forEach(
        (x) => {
            if(x[0].startsWith("reg")){
                buttons[1].classList.remove("disabled");
                buttons[0].classList.add("disabled");
                forms[0].style.display = "none";
                forms[1].style.display = "flex";
            }
            else{
                buttons[0].classList.remove("disabled");
                buttons[1].classList.add("disabled");
                forms[0].style.display = "flex";
                forms[1].style.display = "none";
            }
            error_message.textContent += errors[x[1]]
        }
    );
}
