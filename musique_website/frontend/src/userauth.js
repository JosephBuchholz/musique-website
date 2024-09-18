/**
 * Some user authentication code.
 */

export async function getUser() {
    var user = null;

    await fetch("/user/get/")
        .then(async (response) => {
            if (response.status === 403) {
                response.text().then((text) => {
                    console.log(text);
                });
            } else {
                await response.text().then((text) => {
                    if (text !== "") user = JSON.parse(text);
                    else user = null;
                });
            }
        })
        .catch((error) => {
            console.log(error);
        });

    return user;
}

export async function isAuthenticated() {
    const result = await getUser();
    console.log(`user: ${result}`);
    return result !== null;
}
