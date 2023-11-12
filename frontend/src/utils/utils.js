export default class Subsection {
    constructor(id, label, component, open = false) {
        this.id = id;
        this.label = label;
        this.component = component;
        this.open = open;
    }
}
