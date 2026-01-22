function ValidateAndCalculate(){
    let name = document.getElementById("FullName").value;
    let email = document.getElementById("email").value;
    let numMonitors = parseInt(document.getElementById("numofMonitors").value,10);
    let setupchoice = document.getElementById("setupchoice").innerText;
    let eu = document.getElementById("Europe").checked;
    let outsideEu = document.getElementById("OutsideEu").checked;


    if(!name){
        alert("Please enter your full name");
        return;
    }
    if(!email || !email.includes("@")){
        alert("Please enter your Email(It must contain an '@')");
        return;
    }

    if(isNaN(numMonitors)|| numMonitors <=0){
        alert("Please enter a valid number of monitors");
        return;
    }

    if(setupchoice === "Gaming Setup Options"){
        alert("Please select a valid Setup");
        return;
    }

    if(!eu && !outsideEu){
        alert("Please Select a location");
        return;
    }

    let basePrice;
    if(setupchoice.includes("Low End Setup")){
        basePrice =500;
    } else if(setupchoice.includes("Average Setup")){
        basePrice = 1000;
    }else if(setupchoice.includes("High End Setup")){
        basePrice = 2500;
    }

    let monitorcost = numMonitors * 150;
    let locationtax = eu ? 50: 150;
    let totalPrice = basePrice + monitorcost + locationtax;

    document.getElementById("result").innerHTML = `
        <div class="alert alert-info mt-3">
            <h4>Quotation Detials: </h4>
            <p><strong>Full Name:</strong>${name}</p>
            <p><strong>Email:</strong>${email}</p>
            <p><strong>Setup Choice:</strong>${setupchoice}</p>
            <p><strong>Number of Monitors:</strong>${numMonitors}</p>
            <p><strong>Location:</strong>${eu ? "Europe" : "Outside Europe"}</p>
            <p><strong>Total Price:</strong>€${totalPrice}</p>
        </div>
    `;
    
}

function SelectSetup(option){
    document.getElementById("setupchoice").innerText = option;
}

function validateForm(){
    let fullname = document.getElementById("textbox").value;
    let contactemail = document.getElementById("textboxEmail").value;
    let contactnum = document.getElementById("textboxContactNum").value;
    let comment = document.getElementById("textboxMessage").value;


    if(!fullname || !contactemail || !contactemail || !contactemail.includes("@") || !comment || !contactnum){
        alert("ALL Inputs are Mandatory, please amend your entries and try again.");
        return;
    }
}