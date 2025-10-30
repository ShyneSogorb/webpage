const root = "/public";

export function getAllFirstImages() {
    const images = [];

    Object.keys(import.meta.glob("/public/images/*", { eager: true })).map((img) => {        
        const projectName = img.split(".").at(-2);
        if (!images[projectName]) {
            img = img.slice(root.length);
            images[projectName] = { image: img };
        }
    });

    return images;
}

export function getAllFiles() {
    // Match files under public/docs/was/html (recursive)
    
    const matched = import.meta.glob("/docs/was/html/*", { eager: true });


    console.log(matched);
    
    const ret = Object.keys(matched).map((file) => {
        return "/webpage" + file.slice(root.length);
    });

    return ret;
}