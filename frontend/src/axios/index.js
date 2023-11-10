import axios from "axios";

const espApi = axios.create({
    baseURL: 'http://10.30.0.110/api'
});

export default espApi;
