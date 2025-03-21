const root = "/public"

export function getAllFirstImages()
{

    const images = []
    
    Object.keys(import.meta.glob("/public/images/*", {eager:true})).map(img=>{
        const projectName = img.split(".").at(-2);
        if(!images[projectName])
        {
            img = "/webpage" + img.slice(root.length)
            images[projectName] = { image: img}
        }
    });

    return images;
}