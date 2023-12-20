import { Link } from "react-router-dom";

export default function Header() {
    return (<NavigationBar></NavigationBar>);
}

function NavigationBar() {

  return (
    <>
      <div className="flex bg-gray-100 top-0 w-screen h-16 justify-between">
        <div className="flex justify-center items-center text-center">
            <Link to="/" className="text-3xl font-bold text-black m-6 p-1">Musique</Link>
        </div>

        <ul className="flex justify-center items-center mr-8">
          <NavLink to="/editor">Editor</NavLink>
          <NavLink to="/blog">Blog</NavLink>
        </ul>
      </div>
    </>
  );
}

function NavLink({ to, children }: { to: string, children: string }) {
  return (
    <>
      <li className="text-center m-6 p-1">
        <Link to={to} className="hover:text-blue-600 transition-all font-semibold">{children}</Link>
      </li>
    </>
  );
}
