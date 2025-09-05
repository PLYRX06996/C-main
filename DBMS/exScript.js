const body = document.body;
const btn = document.getElementById("theme-btn");

btn.addEventListener("click", () => {
  body.classList.toggle("dark");
  body.classList.toggle("light");

  if (body.classList.contains("dark")) {
    btn.textContent = "Switch to Light Theme";
    btn.setAttribute("aria-label", "Switch to Light Theme");
  } else {
    btn.textContent = "Switch to Dark Theme";
    btn.setAttribute("aria-label", "Switch to Dark Theme");
  }
});

document.querySelectorAll('.navbar a').forEach(anchor => {
  anchor.addEventListener('click', function(e) {
    e.preventDefault();
    document.querySelector(this.getAttribute('href'))
            .scrollIntoView({ behavior: 'smooth' });
  });
});

const sections = document.querySelectorAll("section, header");
const navLinks = document.querySelectorAll(".navbar a");

window.addEventListener("scroll", () => {
  let current = "";
  sections.forEach(section => {
    const sectionTop = section.offsetTop - 100;
    if (pageYOffset >= sectionTop) {
      current = section.getAttribute("id");
    }
  });

  navLinks.forEach(link => {
    link.classList.remove("active");
    if (link.getAttribute("href") === #${current}) {
      link.classList.add("active");
    }
  });
});

const dots = document.querySelectorAll(".point-nav a");

window.addEventListener("scroll", () => {
  let current = "";
  sections.forEach(section => {
    const sectionTop = section.offsetTop - 200;
    if (pageYOffset >= sectionTop) {
      current = section.getAttribute("id");
    }
  });

  dots.forEach(dot => {
    dot.classList.remove("active-dot");
    if (dot.getAttribute("href") === #${current}) {
      dot.classList.add("active-dot");
    }
  });
});

const timelineItems = document.querySelectorAll(".timeline-item");

const revealTimeline = () => {
  timelineItems.forEach(item => {
    const itemTop = item.getBoundingClientRect().top;
    const windowHeight = window.innerHeight;
    if (itemTop < windowHeight - 100) {
      item.classList.add("visible");
    }
  });
};

window.addEventListener("scroll", revealTimeline);
document.addEventListener("DOMContentLoaded", revealTimeline);

const profilePic = document.getElementById("profile-pic");
const images = ["image/profile.jpg","img1.jpg", "img2.jpg"]; 
let currentIndex = 0;

if (profilePic) {
  profilePic.addEventListener("click", () => {
    currentIndex = (currentIndex + 1) % images.length;
    profilePic.src = images[currentIndex];
    console.log(currentIndex);
  });
}

const collapsibles = document.querySelectorAll(".collapsible");

collapsibles.forEach(card => {
  const header = card.querySelector(".card-header");
  header.addEventListener("click", () => {
    card.classList.toggle("active");
  });
});