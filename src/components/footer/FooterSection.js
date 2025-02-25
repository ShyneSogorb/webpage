export default function FooterSection({Title, Reference = [{href:"", name:""}]}) {
    return (
    <div class="col mb-3 text-center">
        <p>{Title}</p>
        <ul class="nav flex-column">
        {
            Reference.map((v, i)=>{
            return (
                <li key={i} class="nav-item mb-2">
                <a class="nav-link p-0 text-center text-white" 
                href={v.href} target="_blank">
                    {v.name}
                </a>
                </li>
            )
        })
        }
        </ul>
    </div>
    );
  }