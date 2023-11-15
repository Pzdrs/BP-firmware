import axios from "axios";
// if development use 10.30.0.110, in production use localhost
const espApi = axios.create({
    baseURL: `${(process.env.NODE_ENV === 'development' ? 'http://10.30.0.110' : `http://${window.location.hostname}`)}/api`,
});

export default espApi;
