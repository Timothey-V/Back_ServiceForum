open Express;

module Messages = {
 let getAll =
     PromiseMiddleware.from((_next, req, rep) => {
      let queryDict = Request.query(req);
            (
              switch (queryDict->Js.Dict.get("cours")) {
                | Some(c) => {
                MessagesDAO.Messages.getAllByCours(c |> Json_decode.string);
                }
                | None => MessagesDAO.Messages.getAll()
              }
            )
       |> Js.Promise.(
            then_(messagesJson => {
              rep
              |> Response.setHeader("Status", "200")
              |> Response.sendJson(messagesJson)
              |> resolve
            })
          );
     });
 let create =
     PromiseMiddleware.from((_next, req, rep) =>
       Js.Promise.(
         (
             switch (Request.bodyJSON(req)) {
             | None => reject(Failure("INVALID MESSAGE"))
             | Some(reqJson) =>
               switch (
                 reqJson |> Json.Decode.(field("texte", optional(string))),
                 reqJson |> Json.Decode.(field("idCours", optional(string))),
                 reqJson |> Json.Decode.(field("auteur", optional(string))),
               ) {
               | exception e => reject(e)
               | (Some(texte), Some(idCours), Some(auteur)) =>
                         MessagesDAO.Messages.create(
                          texte,
                          idCours,
                          auteur,
                         );
               | _ => reject(Failure("INVALID MESSAGE"))
               }
             }
         )
         |> then_(() => {
              rep
              |> Response.setHeader("Status", "201")
              |> Response.sendJson(
                   Json.Encode.(object_([("success", string("Message created"))])),
                 )
              |> resolve
            })
         |> catch(err => {
              Js.log(err);
              rep
              |> Response.setHeader("Status", "400")
              |> Response.sendJson(
                   Json.Encode.(
                     object_([
                       (
                         "error",
                         string("INVALID REQUEST OR MISSING MESSAGE FIELD"),
                       ),
                     ])
                   ),
                 )
              |> resolve;
            })
       )
     );
};
