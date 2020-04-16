open Express;

module Likes = {

 let create =
     PromiseMiddleware.from((_next, req, rep) =>
       Js.Promise.(
         (
             switch (Request.bodyJSON(req)) {
             | None => reject(Failure("INVALID MESSAGE OR AUTHOR"))
             | Some(reqJson) =>
               switch (
                 reqJson |> Json.Decode.(field("idMessage", optional(int))),
                 reqJson |> Json.Decode.(field("auteur", optional(string))),
               ) {
               | exception e => reject(e)
               | (Some(idMessage), Some(auteur)) =>
                         LikesRepository.Likes.create(
                          idMessage,
                          auteur,
                         );
               | _ => reject(Failure("INVALID MESSAGE OR AUTHOR"))
               }
             }
         )
         |> then_(() => {
              rep
              |> Response.setHeader("Status", "201")
              |> Response.sendJson(
                   Json.Encode.(object_([("success", string("Like created"))])),
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

     let delete =
     PromiseMiddleware.from((_next, req, rep) =>
       Js.Promise.(
         (
             switch (Request.bodyJSON(req)) {
             | None => reject(Failure("INVALID IDMESSAGE OR AUTHOR"))
             | Some(reqJson) =>
               switch (
                reqJson |> Json.Decode.(field("idMessage", optional(int))),
                reqJson |> Json.Decode.(field("auteur", optional(string))),
               ) {
               | exception e => reject(e)
               | (Some(idMessage), Some(auteur)) =>
                         LikesRepository.Likes.delete(
                          idMessage,
                          auteur
                         );
               | _ => reject(Failure("INVALID IDMESSAGE OR AUTHOR"))
               }
             }
         )
         |> then_(() => {
              rep
              |> Response.setHeader("Status", "201")
              |> Response.sendJson(
                   Json.Encode.(object_([("success", string("Like removed"))])),
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
