import load from "@mf-dom/inputgen"

const {SchemaFactory, FieldLabel, FieldType} = await load();

let factory = new SchemaFactory();
let sbuilder = factory.addSchema().withName("JSONCustom");
sbuilder.addField().withName("hello").withLabel(FieldLabel.REQUIRED).withType(FieldType.STRING);
sbuilder.addField().withName("world").withLabel(FieldLabel.REPEATED).withType(FieldType.INT);
sbuilder.addField().withName("foo").withLabel(FieldLabel.OPTIONAL).withType(FieldType.STRING);
console.assert(factory.build());

let mutator = factory.createMutator("JSONCustom", JSON.stringify({"hello": "howdy", "world": [4]}));
let res = [];
let time = performance.now();
for (let i = 0; i < 1000000; i++) {
    res.push(JSON.parse(mutator.getMutation()));
}
time = (performance.now() - time) / 1000.;
let p = document.createElement("p");
p.innerText = time.toString() + " seconds";
document.body.appendChild(p);
console.debug(res)
factory.delete()
