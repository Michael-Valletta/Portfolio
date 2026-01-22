
//Script for ToggleMenu Button in the Contact Us page Using JQuery 
$(document).ready(function() {
    $('#toggleMenu').on("click",function() {
        $('#nv').toggleClass('hidden');
    });
});

//Script for Form Validation
$('#contactForm').on("submit",function(event){
    event.preventDefault();

    $('.error-message').remove();
    let isValid = true;

    const useremail = $('#textboxEmail').val();
    if(!validateEmail(useremail)){
        isValid = false;
        $('#textboxEmail').after('<div class = "error-message">Please Enter a valid Email address</div>')
    }

    const usernumber = $('#textboxContactNum').val();
    if(!validateNumber(usernumber)){
        isValid = false
        $('#textboxContactNum').after('<div class="error-message">Please Enter a Valid Number</div>')
    }

    $('#contactForm input, #contactForm textarea').each(function(){
        if(!$(this).val()){
            isValid = false;
            $(this).after('<div class="error-message">This Field is required</div>');
        }
    });
    if(isValid){
       const fullName = $('#textbox').val();
       const email = $('#textboxEmail').val();
       const contactNumber = $('#textboxContactNum').val();
       const message = $('#textboxMessage').val();

       const mailtoLink = $('#contactForm').attr('action');
       window.location.href = mailtoLink
    };
});

function validateEmail(email){
    var re = /\S+@\S+\.\S+/;
    return re.test(email);
}

function validateNumber(Number){
    var re = /^\d+$/;
    return re.test(Number);
}


//Script for show more button functionality in the Shop Page
document.querySelectorAll('.ShowMore').forEach((button, index) =>{
    button.addEventListener('click',()=> {
        document.querySelectorAll('.bundlesdescription').forEach(desc =>{
            desc.classList.remove('show');
        });
        
        const bundleDescription = document.querySelectorAll('.bundlesdescription')[index];
        if(bundleDescription){
            bundleDescription.classList.toggle('show');

        }else{
            console.error('Bundle Description for Bundle ${index + 1} not found.');
        }
    });
})

// script for add to cart button functionality in the shop page
document.querySelectorAll('.AddToCart').forEach((button, index) =>{
    button.addEventListener('click',() => {
        const bundleNames = document.querySelectorAll('.bundle #bundletitles');
        const bundleName = bundleNames[index * 2].textContent;
        alert(`${bundleName} has been added to Cart`);

    });
});

//Scrip for Fade in images on Home Page
document.addEventListener("DOMContentLoaded", function(){
    const images = document.querySelectorAll(".fade-in");
    images.forEach(function(images){
        images.style.opacity = 1;
    });
});