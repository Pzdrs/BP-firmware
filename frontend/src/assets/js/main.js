import '../css/base.css'
import $ from './jquery-3.7.1.min.js'

$(() => {
    $('button').on('click', (event) => {
        console.log(event.target)
    });
});
