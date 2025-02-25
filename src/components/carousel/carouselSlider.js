import MyProjects from '/public/JSON/projects.json';

export default function SlideTo(){
    //Each folder in images is for a project, so the number of projects is same as number of folders
    return MyProjects.map((v, i)=>{
        var extra = {};
        if(i==0) extra = { class:"active", "aria-current":"true" }
        return <li data-bs-target="#carouselProjects" data-bs-slide-to={i} {...extra}></li>
    })
}