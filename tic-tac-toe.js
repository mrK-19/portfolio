// selecting all required elements
const selectBox = document.querySelector(".select-box");
const selectXBtn = selectBox.querySelector(".playerX");
const selectOBtn = selectBox.querySelector(".playerO");
const playboard = document.querySelector(".playboard");
const allBox = document.querySelectorAll("section span");
const players = document.querySelector(".players");
const resultBox = document.querySelector(".result-box");
const wonText = resultBox.querySelector(".won-text");
const replayBtn = resultBox.querySelector(".btn p");

// once window loaded
window.onload = ()=>{
    for(let i=0; i<allBox.length; i++){
        allBox[i].setAttribute("onclick", "clickedBox(this)");
    }
    selectXBtn.onclick = ()=>{
        selectBox.classList.add("hide");
        playboard.classList.add("show");
    }
    selectOBtn.onclick = ()=>{
        selectBox.classList.add("hide");
        playboard.classList.add("show");
        players.setAttribute("class", "players active player");
    }
}

let playerXIcon = "fas fa-times";  // font awesome
let playerOIcon = "far fa-circle";  // font awesome
let playerSign;  // suppose user will be playerX
let runBot = true;

// user click function
function clickedBox(element){
    //console.log(element);
    if(players.classList.contains("player")){
        // add O icon
        element.innerHTML = `<i class="${playerOIcon}"></i>`;
        players.classList.remove("active");
        playerSign = "O";
        element.setAttribute("id", playerSign);
    }
    else{
        // add X icon
        element.innerHTML = `<i class="${playerXIcon}"></i>`;
        players.classList.add("active");
        playerSign = "X";
        element.setAttribute("id", playerSign);
    }
    selectWinner();  // call winner function
    playboard.style.pointerEvents = "none";  // avert clicking when it's not user's turn
    element.style.pointerEvents = "none";  // avert clicking twice
    
    // random delay for bot's turn
    let randomDelayTime = ((Math.random() * 1000) + 200).toFixed();
    setTimeout(()=>{
        bot(runBot);  // call bot function
    }, randomDelayTime);
}

// bot click function
function bot(runBot){
    if(runBot){
        let array = [];  // store unselected box index
        for(let i=0; i<allBox.length; i++){
            if(allBox[i].childElementCount == 0){
                array.push(i);
                //console.log(i + " has no children");
            }
        }
        // bot will choose unselected box index randomly
        let randomBox = array[Math.floor(Math.random() * array.length)];
        if(array.length > 0){
            if(players.classList.contains("player")){
                // add X icon
                allBox[randomBox].innerHTML = `<i class="${playerXIcon}"></i>`;
                players.classList.add("active");
                playerSign = "X";
                allBox[randomBox].setAttribute("id", playerSign);
            }
            else{
                // add O icon
                allBox[randomBox].innerHTML = `<i class="${playerOIcon}"></i>`;
                players.classList.remove("active");
                playerSign = "O";
                allBox[randomBox].setAttribute("id", playerSign);
            }
            selectWinner();  // call winner function
        }
        playboard.style.pointerEvents = "auto";  // unlock user's pointer event
        allBox[randomBox].style.pointerEvents = "none";  // avert clicking twice
    }
}

// let work on select winner
function getId(idName){
    return document.querySelector(".box" + idName).id;
}

function checkThreeIds(val1, val2, val3){
    if(getId(val1) == playerSign && getId(val2) == playerSign && getId(val3) == playerSign){
        return true;
    }
    else return false;
}

// list of combos
const combos = [
    [1, 2, 3], [4, 5, 6], [7, 8, 9], 
    [1, 4, 7], [2, 5, 8], [3, 6, 9], 
    [1, 5, 9], [3, 5 ,7]
];

function selectWinner(){
    for(let i=0; i<combos.length; i++){
        if(checkThreeIds(combos[i][0], combos[i][1], combos[i][2])){
            //console.log(playerSign + " is the winner!");
            runBot = false;
            setTimeout(()=>{
                playboard.classList.remove("show");
                resultBox.classList.add("show");
                wonText.innerHTML = `Player <p>${playerSign}</p> won!`;
            }, 700);
            return;
        }
    }
    let cnt = 0;
    for(let i=1; i<=allBox.length; i++){
        if(getId(i) != "") cnt++;
    }
    if(cnt == allBox.length){
        runBot = false;
        setTimeout(()=>{
            playboard.classList.remove("show");
            resultBox.classList.add("show");
            wonText.textContent = "The match has been drawn.";
        }, 700);
        return;
    }
}

// show playboard again if replay button pressed
replayBtn.onclick = ()=>{
    window.location.reload();
};
